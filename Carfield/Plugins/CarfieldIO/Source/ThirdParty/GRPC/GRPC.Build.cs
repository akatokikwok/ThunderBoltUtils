// Copyright Epic Games, Inc. All Rights Reserved.

/*
	模块说明:
	该模块表示GRPC相关功能，使用到的文件均存在于模块所在文件夹外层同级目录下的ThirdParty内
 
 */

using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Net.NetworkInformation;
using System.Text;
using System.Text.RegularExpressions;
using UnrealBuildTool;

public class GRPC : ModuleRules
{
    

    private string ModulePath
    {
        get { return Path.GetFullPath( ModuleDirectory); }
    }
    // 访问模块下的ThirdParty路径
    private string ThirdPartyPath
    {
        get { return Path.GetFullPath(Path.Combine(ModulePath, "ThirdParty")); }
    }

    // 所需要的第三方库名称集合
    protected List<string> GrpcLibsCollection = new List<string>()
    {
        "absl_bad_optional_access",
        "absl_bad_variant_access",
        "absl_base",
        "absl_city",
        "absl_civil_time",
        "absl_cord",
        "absl_debugging_internal",
        "absl_demangle_internal",
        "absl_exponential_biased",
        "absl_graphcycles_internal",
        "absl_hash",
        "absl_hashtablez_sampler",
        "absl_int128",
        "absl_log_severity",
        "absl_malloc_internal",
        "absl_raw_hash_set",
        "absl_raw_logging_internal",
        "absl_spinlock_wait",
        "absl_stacktrace",
        "absl_status",
        "absl_statusor",
        "absl_str_format_internal",
        "absl_strings",
        "absl_strings_internal",
        "absl_symbolize",
        "absl_synchronization",
        "absl_throw_delegate",
        "absl_time",
        "absl_time_zone",
        "address_sorting",
        "cares",
        "crypto",
        "gpr",
        "grpc",
        "grpc_plugin_support",
        "grpc_unsecure",
        "grpc++",
        "grpc++_alts",
        "grpc++_error_details",
        "grpc++_reflection",
        "grpc++_unsecure",
        "grpcpp_channelz",
        "libprotobuf",
        "libprotobuf-lite",
        "libprotoc",
        "re2",
        "ssl",
        "upb",
        "zlib",
        "zlibstatic",

    };

    public GRPC(ReadOnlyTargetRules Target) : base(Target)
    {
        Type = ModuleType.External;

        //PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;       
         
        PublicDefinitions.Add("GOOGLE_PROTOBUF_NO_RTTI");
        PublicDefinitions.Add("GPR_FORBID_UNREACHABLE_CODE");
        PublicDefinitions.Add("GRPC_ALLOW_EXCEPTIONS=0");

        //TODO: We do this because in file generated_message_table_driven.h that located in protobuf sources 
        //TODO: line 174: static_assert(std::is_pod<AuxillaryParseTableField>::value, "");
        //TODO: causes С4647 level 3 warning __is_pod behavior change
        //TODO: UE4 threading some warnings as errors, and we have no chance to suppress this stuff
        //TODO: So, we don't want to change any third-party code, this why we add this definition
        PublicDefinitions.Add("__NVCC__");

        #region 弃用
        //Platform = Target.Platform;
        //Configuration = Target.Configuration;
        //ModuleDepPaths moduleDepPaths = GatherDeps();
        //Console.WriteLine(moduleDepPaths.ToString());
        //PublicIncludePaths.AddRange(moduleDepPaths.HeaderPaths);
        #endregion

        PublicIncludePaths.AddRange(
            new string[] {
                // ... add public include paths required here ...
                Path.Combine(ModulePath, "include"),
            }
        );

        PrivateIncludePaths.AddRange(
            new string[] {
				// ... add other private include paths required here ...
			}
        );

        // 访问到模块下第三方的lib文件夹并开始第三方库链接
        string LibPath = Path.Combine(ModulePath, "lib/Win64");
        var ConvertedLibPaths = GrpcLibsCollection.ConvertAll(m => Path.Combine(LibPath, /*"lib" +*/ m + ".lib"));
        PublicAdditionalLibraries.AddRange(ConvertedLibPaths);

        #region 弃用 
        //PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "lib", "absl_bad_optional_access.lib"));
        //PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "lib", "absl_bad_variant_access.lib"));
        //PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "lib", "absl_base.lib"));
        //PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "lib", "absl_city.lib"));
        //PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "lib", "absl_civil_time.lib"));
        //PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "lib", "absl_cord.lib"));
        //PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "lib", "absl_debugging_internal.lib"));
        //PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "lib", "absl_demangle_internal.lib"));
        //PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "lib", "absl_exponential_biased.lib"));
        //PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "lib", "absl_graphcycles_internal.lib"));
        //PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "lib", "absl_hash.lib"));
        //PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "lib", "absl_hashtablez_sampler.lib"));
        //PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "lib", "absl_int128.lib"));
        //PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "lib", "absl_log_severity.lib"));
        //PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "lib", "absl_malloc_internal.lib"));
        //PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "lib", "absl_raw_hash_set.lib"));
        //PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "lib", "absl_raw_logging_internal.lib"));
        //PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "lib", "absl_spinlock_wait.lib"));
        //PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "lib", "absl_stacktrace.lib"));
        //PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "lib", "absl_status.lib"));
        //PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "lib", "absl_statusor.lib"));
        //PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "lib", "absl_str_format_internal.lib"));
        //PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "lib", "absl_strings.lib"));
        //PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "lib", "absl_strings_internal.lib"));
        //PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "lib", "absl_symbolize.lib"));
        //PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "lib", "absl_synchronization.lib"));
        //PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "lib", "absl_throw_delegate.lib"));
        //PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "lib", "absl_time.lib"));
        //PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "lib", "absl_time_zone.lib"));
        //PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "lib", "address_sorting.lib"));
        //PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "lib", "cares.lib"));
        //PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "lib", "crypto.lib"));
        //PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "lib", "gpr.lib"));
        //PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "lib", "grpc.lib"));
        //PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "lib", "grpc_plugin_support.lib"));
        //PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "lib", "grpc_unsecure.lib"));
        //PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "lib", "grpc++.lib"));
        //PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "lib", "grpc++_alts.lib"));
        //PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "lib", "grpc++_error_details.lib"));
        //PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "lib", "grpc++_reflection.lib"));
        //PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "lib", "grpc++_unsecure.lib"));
        //PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "lib", "grpcpp_channelz.lib"));
        //PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "lib", "libprotobuf.lib"));
        //PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "lib", "libprotobuf-lite.lib"));
        //PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "lib", "libprotoc.lib"));
        //PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "lib", "re2.lib"));
        //PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "lib", "ssl.lib"));
        //PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "lib", "upb.lib"));
        //PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "lib", "zlib.lib"));
        //PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "lib", "zlibstatic.lib"));
        #endregion

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core"
        });

        // 工程需要的额外依赖库 
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
