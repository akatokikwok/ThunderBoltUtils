/*
 
 */

using System;
using System.Collections.Generic;
using System.IO;
using System.Net.NetworkInformation;
using System.Text;
using System.Text.RegularExpressions;
using UnrealBuildTool;

public class carfield_map_api : ModuleRules
{
    private string ModulePath
    {
        get { return Path.GetFullPath(ModuleDirectory); }
    }

    // ����ģ���µ�ThirdParty·��(���Ϸ���2��)
    private string ThirdPartyPath
    {
        get { return Path.GetFullPath(Path.Combine(ModulePath, "../../ThirdParty/")); }
    }

    // Linux��Ҫ�ĵ����������Ƽ���,�洢��һ��List��
    protected List<string> Linuxcarfield_map_apiLibsCollection = new List<string>()
    {
        "libcarfield_map_api",

    };

    public carfield_map_api(ReadOnlyTargetRules Target) : base(Target)
    {
        Type = ModuleType.External;

        /* Linuxƽ̨�µ������ֶ�*/
        if (Target.Platform == UnrealTargetPlatform.Linux)
        {
            PublicDefinitions.Add("__NVCC__");

            PublicIncludePaths.AddRange(
               new string[] {
                // ... add public include paths required here ...
                Path.Combine(ModulePath, "include"),
               }
            );


            string LibPath = Path.Combine(ModulePath, "lib/Linux");
            var ConvertedLibPaths = Linuxcarfield_map_apiLibsCollection.ConvertAll(m => Path.Combine(LibPath, m + ".so"));
            PublicAdditionalLibraries.AddRange(
               ConvertedLibPaths                
            );
            RuntimeDependencies.Add("$(ProjectDir)/lib/Linux/libcarfield_map_api.so");
            PrivateRuntimeLibraryPaths.Add("$(ProjectDir)/lib/");


            //// ���ʵ�ģ���µ�������lib�ļ��в���ʼ������������
            //string LibPath = Path.Combine(ModulePath, "lib/Linux");
            //var ConvertedLibPaths = LinuxGrpcLibsCollection.ConvertAll(m => Path.Combine(LibPath, /*"lib" +*/ m + ".a"));
            //PublicAdditionalLibraries.AddRange(ConvertedLibPaths);

            PublicDependencyModuleNames.AddRange(new string[] {
            "Core","OpenSSL",
            });

            // ������Ҫ�Ķ����������������� 
            AddEngineThirdPartyPrivateStaticDependencies(Target, "OpenSSL");
            AddEngineThirdPartyPrivateStaticDependencies(Target, "zlib");

            PrivateDependencyModuleNames.AddRange(
                new string[]
                {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                    // ... add private dependencies that you statically link with here ...	
                }
                );

            PrivateIncludePaths.AddRange(
                new string[] {
                    // ... add other private include paths required here ...
                }
                );


            PublicDependencyModuleNames.AddRange(
                new string[]
                {
                "Core",
                    // ... add other public dependencies that you statically link with here ...
                }
                );

            DynamicallyLoadedModuleNames.AddRange(
                new string[]
                {
                    // ... add any modules that your module loads dynamically here ...
                }
                );



        }

    }

}