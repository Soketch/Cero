function(force_redefine_file_macro_for_sources targetname)
    # 获取目标的所有源文件列表
    get_target_property(source_files "${targetname}" SOURCES)
    
    # 遍历每个源文件
    foreach(sourcefile ${source_files})
        # 获取源文件当前的编译定义（COMPILE_DEFINITIONS）
        get_property(defs SOURCE "${sourcefile}" PROPERTY COMPILE_DEFINITIONS)
        
        # 获取源文件在项目目录中的相对路径
        get_filename_component(filepath "${sourcefile}" ABSOLUTE)
        string(REPLACE ${PROJECT_SOURCE_DIR}/ "" relpath ${filepath})  # 替换掉项目目录路径，得到相对路径
        
        # 将相对路径作为宏定义添加到源文件的定义列表中
        list(APPEND defs "__FILE__=\"${relpath}\"")
        
        # 更新源文件的编译定义
        set_property(
            SOURCE "${sourcefile}"
            PROPERTY COMPILE_DEFINITIONS ${defs}
        )
    endforeach()
endfunction()
