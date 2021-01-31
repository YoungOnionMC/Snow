project "ExampleScriptApp"
    kind "SharedLib"
    language "C#"
    
    targetdir ("%{wks.location}/Glacier/assets/scripts")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "src/**.cs",
    }

    links {
        "SnowScriptCore",
    }