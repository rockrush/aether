#include <iostream>
#include <stdlib.h>
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

VkResult create_vk_instance(VkInstance *instance) {
	VkApplicationInfo app_info = {};
	VkInstanceCreateInfo instance_info = {};

	app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	app_info.pNext = nullptr;
	app_info.pApplicationName = "Aether Network Simulator";
	app_info.applicationVersion = 1;
	app_info.pEngineName = "Aether";
	app_info.engineVersion = 1;
	app_info.apiVersion = VK_MAKE_VERSION(1, 0, 51);

	instance_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instance_info.pNext = nullptr;
	instance_info.flags = 0;
	instance_info.pApplicationInfo = &app_info;
	instance_info.enabledLayerCount = 0;
	instance_info.ppEnabledLayerNames = nullptr;
	instance_info.enabledExtensionCount = 0;
	instance_info.ppEnabledExtensionNames = nullptr;

	return vkCreateInstance(&instance_info, nullptr, instance);
}

VkResult enumerate_phys_devs(VkInstance *inst, VkPhysicalDevice *phys_devs, uint32_t *cnt) {
	uint32_t dev_cnt = 0;
	VkResult vk_ret = vkEnumeratePhysicalDevices(*inst, &dev_cnt, nullptr);

	if ((vk_ret != VK_SUCCESS) || (dev_cnt == 0)) {
		std::cerr << "(Unknown error occured enumerating physical devices) " << __func__ << ":" << __LINE__ << std::endl;
		return VK_ERROR_DEVICE_LOST;
	}

	std::cout << "Total physical devices: " << dev_cnt << std::endl;
	phys_devs = (VkPhysicalDevice *)malloc(dev_cnt * sizeof(VkPhysicalDevice));
	if (phys_devs == NULL) {
		std::cerr << "(Failed allocate memory) " << __func__ << ":" << __LINE__ << std::endl;
		return VK_ERROR_OUT_OF_HOST_MEMORY;
	}

	std::cout << "Memory allocated: " << dev_cnt * sizeof(VkPhysicalDevice) << std::endl;
	vk_ret = vkEnumeratePhysicalDevices(*inst, &dev_cnt, phys_devs);
	if ((vk_ret != VK_SUCCESS) || (dev_cnt == 0)) {
		std::cerr << "(Unknown error occured) " << __func__ << ":" << __LINE__ << std::endl;
		free(phys_devs);
		return VK_ERROR_DEVICE_LOST;
	}

	*cnt = dev_cnt;
	return VK_SUCCESS;
}

int main(int argc, char *argv[]) {
	VkInstance instance;
	VkResult vk_ret = VK_SUCCESS;
	uint32_t phys_dev_cnt = 0;
	VkPhysicalDevice *phys_devs = nullptr;

	vk_ret = create_vk_instance(&instance);
	if (vk_ret != VK_SUCCESS) {
		std::cerr << "Instance Creation failed" << __func__ << ":" << __LINE__ << std::endl;
		return EXIT_FAILURE;
	}

	vk_ret = enumerate_phys_devs(&instance, phys_devs, &phys_dev_cnt);
	if (vk_ret == VK_SUCCESS) {
		free(phys_devs);
	} else {
		std::cout << "No compatible physical device found" << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
