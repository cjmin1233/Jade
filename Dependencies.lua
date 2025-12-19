-- Jade Dependencies
VULKAN_SDK = os.getenv("VULKAN_SDK")    -- Get Vulkan SDK path from environment variable

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/Jade/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/Jade/vendor/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/Jade/vendor/imgui"
IncludeDir["glm"] = "%{wks.location}/Jade/vendor/glm"
IncludeDir["stb_image"] = "%{wks.location}/Jade/vendor/stb_image"
IncludeDir["entt"] = "%{wks.location}/Jade/vendor/entt/include"
IncludeDir["yaml_cpp"] = "%{wks.location}/Jade/vendor/yaml-cpp/include"
IncludeDir["ImGuizmo"] = "%{wks.location}/Jade/vendor/ImGuizmo"
IncludeDir["shaderc"] = "%{wks.location}/Jade/vendor/shaderc/include"
IncludeDir["SPIRV_Cross"] = "%{wks.location}/Jade/vendor/SPIRV-Cross"
IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/Include"

LibraryDir = {}

LibraryDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"

Library = {}
Library["Vulkan"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"

Library["ShaderC_Debug"] = "%{LibraryDir.VulkanSDK}/shaderc_sharedd.lib"
Library["SPIRV_Cross_Debug"] = "%{LibraryDir.VulkanSDK}/spirv-cross-cored.lib"
Library["SPIRV_Cross_GLSL_Debug"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsld.lib"

Library["ShaderC_Release"] = "%{LibraryDir.VulkanSDK}/shaderc_shared.lib"
Library["SPIRV_Cross_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-core.lib"
Library["SPIRV_Cross_GLSL_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsl.lib"
