#include "render.h"
#include "g_defs.h"
#include "utils/defs.h"
#include "utils/logger.h"
#include "vulkan/vulkan_core.h"
#include <SDL_vulkan.h>
#include <string.h>
#include <vulkan/vulkan.h>

#ifndef DEBUG
const bool validation_layers_active = true;
#else
const bool validation_layers_active = false;
#endif

#ifndef MACOS
const bool macos_extensions_active = true;
#else
const bool macos_extensions_active = false;
#endif

static int vk_create_logical_device(global *g) {
  float queuePriority = 1.0f;
  VkDeviceQueueCreateInfo ldev_queue_info = {
      .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
      .pNext = NULL,
      .queueFamilyIndex = g->vk.queues.family_indices.graphics_family,
      .queueCount = 1,
      .pQueuePriorities = &queuePriority,
  };

  VkPhysicalDeviceFeatures pdev_feats = {};

  // TODO fix on other platforms
  const char *names[1];
  names[0] = "VK_KHR_portability_subset";

  VkDeviceCreateInfo ldev_create_info = {
      .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
      .pNext = NULL,
      .flags = 0,
      .pQueueCreateInfos = &ldev_queue_info,
      .queueCreateInfoCount = 1,
      .pEnabledFeatures = &pdev_feats,
      .enabledLayerCount = 0,
      .ppEnabledLayerNames = NULL,
      .enabledExtensionCount = 1,
      .ppEnabledExtensionNames = names,
  };

  if (validation_layers_active) {
    ldev_create_info.enabledLayerCount = g->vk.layers.count;
    ldev_create_info.ppEnabledLayerNames = g->vk.layers.names;
  }

  if (vkCreateDevice(g->vk.pdevice, &ldev_create_info, NULL, &g->vk.ldevice) !=
      VK_SUCCESS) {
    return -1;
  }
  LOG("VK LOGIKAL DEVICE CREATED\n");

  vkGetDeviceQueue(g->vk.ldevice, g->vk.queues.family_indices.graphics_family,
                   0, &g->vk.queues.graphics_queue);
  LOG("VK LOGICAL DEVICE QUEUE CREATED\n");

  return 0;
}

static bool vk_queue_indices_is_complete(global *g) {
  if (g->vk.queues.family_indices.graphics_family != NO_INDEX)
    return true;
  else
    return false;
}

static void vk_find_queue_families(global *g, VkPhysicalDevice pdev) {
  u32 queue_fam_count;
  vkGetPhysicalDeviceQueueFamilyProperties(pdev, &queue_fam_count, NULL);
  VkQueueFamilyProperties queue_fam_props[queue_fam_count];
  vkGetPhysicalDeviceQueueFamilyProperties(pdev, &queue_fam_count,
                                           queue_fam_props);

  LOG("%d queue families:\n", queue_fam_count);
  for (u32 i = 0; i < queue_fam_count; i++) {
    if (queue_fam_props[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      g->vk.queues.family_indices.graphics_family = i;
      LOG("\t%d : loaded\n", queue_fam_props[i].queueCount);
      if (vk_queue_indices_is_complete(g)) {
        LOG("all needed queue family indices found\n");
        break;
      }
    } else {
      LOG("\t%d : not loaded\n", queue_fam_props[i].queueCount);
    }
  }
}

static bool vk_is_device_suitable(global *g, VkPhysicalDevice pdev,
                                  VkInstance vkinst) {
  // TODO rating system
  // TODO integrated gpu has only on mac -> should be discrete
  VkPhysicalDeviceProperties pdev_props;
  VkPhysicalDeviceFeatures pdev_feats;

  vkGetPhysicalDeviceProperties(pdev, &pdev_props);
  vkGetPhysicalDeviceFeatures(pdev, &pdev_feats);

  vk_find_queue_families(g, pdev);

  if (pdev_props.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU &&
      vk_queue_indices_is_complete(g)) {
    LOG("Device (%s) is suitable and will be used\n", pdev_props.deviceName);
    return true;
  } else {
    return false;
  }
}

static int vk_pick_pDevice(global *g, VkInstance vkinst) {
  u32 physical_devices_count;
  vkEnumeratePhysicalDevices(vkinst, &physical_devices_count, NULL);
  if (physical_devices_count == 0) {
    ERR("Couldn't find any GPU\n");
    return -1;
  }

  VkPhysicalDevice physical_devices[physical_devices_count];
  vkEnumeratePhysicalDevices(vkinst, &physical_devices_count, physical_devices);

  // TODO rating system
  for (usize i = 0; i < physical_devices_count; i++) {
    if (vk_is_device_suitable(g, physical_devices[i], vkinst)) {
      g->vk.pdevice = physical_devices[i];
      break;
    }
  }

  if (g->vk.pdevice == VK_NULL_HANDLE) {
    ERR("Couldn't find suitable GPU\n");
    return -1;
  }
  return 0;
}

static int vk_check_validation_layers(u32 layer_count,
                                      const char **layer_names) {
  u32 inst_layer_count;
  vkEnumerateInstanceLayerProperties(&inst_layer_count, NULL);
  VkLayerProperties inst_layer_props[inst_layer_count];
  vkEnumerateInstanceLayerProperties(&inst_layer_count, inst_layer_props);

  LOG("checking %d validation layer(s):\n", layer_count);
  for (usize i = 0; i < layer_count; i++) {
    bool found = false;
    for (usize a = 0; a < inst_layer_count; a++) {
      if (strcmp(layer_names[i], inst_layer_props[i].layerName)) {
        found = true;
        break;
      } else {
        continue;
      }
    }

    if (found) {
      LOG("\t- %s : found\n", layer_names[i]);
    } else {
      ERR("\t- %s : not found; exiting\n", layer_names[i]);
      return -1;
    }
  }
  LOG("validation Layer(s) found\n");
  return 0;
}

static int vk_create_validation_layers(global *g) {
  // TODO proper DEBUGGER
  const u32 validation_layer_count = 1;
  const char *validation_layer_names[validation_layer_count];

  // custom validation layer names
  validation_layer_names[0] = "VK_LAYER_KHRONOS_validation";

  if (vk_check_validation_layers(validation_layer_count,
                                 validation_layer_names) != 0) {
    return -1;
  }
  g->vk.layers.count = validation_layer_count;
  g->vk.layers.names = validation_layer_names;
  return 0;
}

static int vk_check_extensions(u32 ext_count, const char **ext_names) {
  u32 inst_ext_count;
  vkEnumerateInstanceExtensionProperties(NULL, &inst_ext_count, NULL);
  VkExtensionProperties inst_ext_props[inst_ext_count];
  vkEnumerateInstanceExtensionProperties(NULL, &inst_ext_count, inst_ext_props);

  LOG("checking %d extension(s):\n", ext_count);
  for (usize i = 0; i < ext_count; i++) {
    bool found = false;
    for (usize a = 0; a < inst_ext_count; a++) {
      if (strcmp(ext_names[i], inst_ext_props[i].extensionName)) {
        found = true;
        break;
      } else {
        continue;
      }
    }

    if (found) {
      LOG("\t- %s : found\n", ext_names[i]);
    } else {
      ERR("\t- %s : not found; exiting\n", ext_names[i]);
      return -1;
    }
  }
  LOG("Extension(s) found\n");
  return 0;
}

int vk_find_extensions(global *g) {
  if (macos_extensions_active) {
    u32 extension_count;
    SDL_Vulkan_GetInstanceExtensions(g->sdl.window, &extension_count, NULL);

    const char *extension_names[extension_count + 2];
    SDL_Vulkan_GetInstanceExtensions(g->sdl.window, &extension_count,
                                     extension_names);
    extension_count += 2;

    // macos portability options REQUIRED
    extension_names[extension_count - 2] = "VK_KHR_portability_enumeration";
    extension_names[extension_count - 1] =
        "VK_KHR_get_physical_device_properties2";

    if (vk_check_extensions(extension_count, extension_names) != 0) {
      return -1;
    }

    g->vk.extensions.count = extension_count;
    g->vk.extensions.names = extension_names;

  } else {
    LOG("here\n");
    const char *extension_names[g->vk.extensions.count];
    SDL_Vulkan_GetInstanceExtensions(g->sdl.window, &g->vk.extensions.count,
                                     extension_names);

    g->vk.extensions.names = extension_names;
  }

  return 0;
}

int vk_init(global *g, VkInstance vkinst) {
  const VkApplicationInfo app_info = (VkApplicationInfo){
      .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
      .pNext = NULL,
      .pApplicationName = APPLICATION_NAME,
      .applicationVersion = APPLICATION_VERSION,
      .pEngineName = ENGINE_NAME,
      .engineVersion = ENGINE_VERSION,
      .apiVersion = VK_API_VERSION_1_3,
  };

  // vkextensions
  if (vk_find_extensions(g) != 0) {
    vk_quit(g);
    return -1;
  }

  VkInstanceCreateInfo create_info = (VkInstanceCreateInfo){
      .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
      .pNext = NULL,
      .flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR,
      .pApplicationInfo = &app_info,
      .enabledLayerCount = 0,
      .ppEnabledLayerNames = NULL,
      .enabledExtensionCount = g->vk.extensions.count,
      .ppEnabledExtensionNames = g->vk.extensions.names,
  };

  // enable validation layers
  if (validation_layers_active) {
    if (vk_create_validation_layers(g) != 0) {
      vk_quit(g);
      return -1;
    }
    create_info.enabledLayerCount = g->vk.layers.count;
    create_info.ppEnabledLayerNames = g->vk.layers.names;
  }

  VkResult instance_result = vkCreateInstance(&create_info, NULL, &vkinst);
  if (instance_result != VK_SUCCESS) {
    ERR("vkInstance couldn't get created: error: %d\n", instance_result);
    vk_quit(g);
    return -1;
  }

  if (vk_pick_pDevice(g, vkinst) != 0) {
    vk_quit(g);
    return -1;
  }

  if (vk_create_logical_device(g) != 0) {
    vk_quit(g);
    return -1;
  }

  LOG("VK INITIALIZED\n");
  return 0;
}

void vk_quit(global *g) {
  vkDestroyDevice(g->vk.ldevice, NULL);
  LOG("VK LDEVICE DESTROYED\n");
  vkDestroyInstance(g->vk.inst, NULL);
  LOG("VK INSTANCE DESTROYED\n");
  SDL_Vulkan_UnloadLibrary();
  LOG("SDL_VULKAN LIBRARY UNLOADED\n");
}
