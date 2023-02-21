set(TARGET "test_bgfx")

# juce_add_gui_app(${TARGET}
#     PRODUCT_NAME "Test BGFX"
#     VERSION "1.0.0"
#     COMPANY_NAME "Arthur Benilov"
# )

juce_add_plugin(${TARGET} 
    PRODUCT_NAME "Test BGFX"
    VERSION "1.0.0"
    COMPANY_NAME "Arthur Benilov"
    FORMATS AU VST3 Standalone
)

target_link_libraries(${TARGET}
    PRIVATE
        juce::juce_core
        juce::juce_data_structures
        juce::juce_audio_basics
        juce::juce_audio_utils
        juce::juce_audio_plugin_client
        juce::juce_gui_basics
        juce::juce_gui_extra
    PUBLIC
        juce::juce_recommended_config_flags
)

juce_generate_juce_header(${TARGET})

file(GLOB_RECURSE src
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

# target_sources(${TARGET}
#     PRIVATE
#         PluginEditor.cpp
#         PluginProcessor.cpp)
target_sources(${TARGET} PRIVATE ${src})

file(GLOB_RECURSE res "${CMAKE_CURRENT_SOURCE_DIR}/resources/*.*")
juce_add_binary_data(${TARGET}_res SOURCES ${res})
target_link_libraries(${TARGET} PUBLIC ${TARGET}_res)

target_compile_definitions(${TARGET}
    PUBLIC
        JUCE_DISPLAY_SPLASH_SCREEN=0
        # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
        JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_plugin` call
        JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
        JUCE_VST3_CAN_REPLACE_VST2=0
)


target_link_libraries(${TARGET}
    PRIVATE
        bx
        bimg
        bgfx
        nanovg
)
