cmake_minimum_required (VERSION 2.6)
project(BasisCryptAnalysisGUI)
add_executable(basicCryptAnalysisGUI main.cpp affine.cpp contact.cpp english.cpp frequency.cpp subst.cpp vigenere.cpp)
target_link_libraries(basicCryptAnalysisGUI wx_gtk2u_richtext-2.8 wx_gtk2u_aui-2.8 wx_gtk2u_xrc-2.8 wx_gtk2u_qa-2.8 wx_gtk2u_html-2.8 wx_gtk2u_adv-2.8 wx_gtk2u_core-2.8 wx_baseu_xml-2.8 wx_baseu_net-2.8 wx_baseu-2.8)
include_directories(/usr/include/wx-2.8 /usr/lib/wx/include/gtk2-unicode-release-2.8)
add_definitions(-D__WXGTK__)
