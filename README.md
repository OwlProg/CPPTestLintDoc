# CPPTestLintDoc

# Getting Started
1. Download and add source code of the tool to your project.  
2. Add the following lines to your CMakeLists.txt:
>```CMake
>add_subdirectory(CPPTestLintDoc)
>add_executable(DocGen ${DOCGEN_FILES})
>```
### Usage
#### Autodocumentation
Converts all documentation in form of comments in the code into a set of html pages.  
1.Fill in all fields of config file:
>```
># Please, specify paths in quotation marks
>ProjectName:
>  %PROJECT_NAME%
>
>RootPath:
>  "%ROOT_PATH%"
>
>LogoPath:
>  "%LOGO_PATH%"
>
># Enter a path to the directory where you want to save generated documentation
>DocumentaionPath:
>  "%DOCUMENTATION_PATH%"
>
># You can enumerate files via [*FileName1*, *FileName2*, ...] or using
># - *FileName1*
># - *FileName2*
># - ...
>FilesToProcessPaths:
>  - "%FILE_NAME1%"
>  - "%FILE_NAME2%"
>
>RepositoryURL:
>  %REPOSITORY_URL%
>
># Put the paths to the files you want to be processed
># by linter here as a list (via "-" or via "[]")
>FilesToLinterCheck:
>  - "%FILE_NAME1%"
>  - "%FILE_NAME2%"
>```
2.Build and run target DocGen. Documentation will be generated in the directory which you specified in config.
> You can easily use it in GitHub Pages. Just choose a documentation directory as a source in Settings -> Pages.
>
# Examples