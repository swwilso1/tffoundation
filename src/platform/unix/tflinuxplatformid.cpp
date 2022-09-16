/******************************************************************************

Tectiform Open Source License (TOS)

Copyright (c) 2022 to 2022 Tectiform Inc.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
SOFTWARE.


******************************************************************************/

#define NEEDS_SYS_SYSINFO_H
#include "tfheaders.hpp"
#include "tfplatformid.hpp"
#include "tfposixplatformutilities.hpp"
#include "tfconversion.hpp"

#if defined(__GNUC__)
#    if defined(__GNUC_PATCHLEVEL__)
#        define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#    else
#        define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100)
#    endif
#endif

namespace TF::Foundation
{
    void PlatformId::initialize()
    {
        s_name = "Linux";

        auto rpm_path = find_path_to_binary("rpm");
        auto lsb_release_path = find_path_to_binary("lsb_release");

        if (rpm_path)
        {
            auto centos_query_result = run_command(rpm_path.value() + " -q centos-release");
            auto fedora_query_result = run_command(rpm_path.value() + " -q fedora-release");
            auto sles_query_result = run_command(rpm_path.value() + " -q sles-release");
            auto system_query_result = run_command(rpm_path.value() + " -q system-release");
            String release_string{};

            std::vector<String> release_helper_strings{};
            release_helper_strings.emplace_back("redhat-release");
            release_helper_strings.emplace_back("redhat-release-server");
            release_helper_strings.emplace_back("redhat-release-client");
            release_helper_strings.emplace_back("redhat-release-computenode");
            release_helper_strings.emplace_back("redhat-release-workstation");

            for (auto & helper : release_helper_strings)
            {
                auto rhel_query_result = run_command(rpm_path.value() + " -q " + helper);
                if (rhel_query_result.length() > 0 && rhel_query_result != "not installed")
                {
                    s_vendor = "Redhat";
                    release_string = helper;
                }
            }

            if (s_vendor.length() == 0)
            {
                if (centos_query_result != "not installed")
                {
                    s_vendor = "CentOS";
                    release_string = "centos-release";
                }
                else if (fedora_query_result != "not installed")
                {
                    s_vendor = "Fedora";
                    release_string = "fedora-release";
                }
                else if (sles_query_result != "not installed")
                {
                    s_vendor = "Suse";
                    release_string = "sles-release";
                }
                else if (system_query_result != "not installed")
                {
                    auto sub_vendor_query_result =
                        run_command(rpm_path.value() + " -q --qf \"%{VENDOR}\" system-release");
                    if (sub_vendor_query_result.contains("Amazon"))
                    {
                        s_vendor = "Amazon";
                        release_string = "system-release";
                    }
                }

                auto major_version = Conversion::convertStringToInt(
                    run_command(rpm_path.value() + " -q -qf \"%{VERSION}\" " + release_string));
                auto minor_version = Conversion::convertStringToInt(
                    run_command(rpm_path.value() + " -q -qf \"%{RELEASE}\" " + release_string));

                if (major_version)
                {
                    s_version.major = static_cast<decltype(s_version.major)>(major_version.value());
                }

                if (minor_version)
                {
                    s_version.minor = static_cast<decltype(s_version.minor)>(minor_version.value());
                }
            }
        }
        else if (lsb_release_path)
        {
            auto distribution = run_command(lsb_release_path.value() + " -i");
            auto release = run_command(lsb_release_path.value() + " -r");

            auto distribution_parts = distribution.split(":");
            if (distribution_parts.size() > 1)
            {
                if (distribution_parts[1].contains("Ubuntu"))
                {
                    s_vendor = "Ubuntu";
                }
                else if (distribution_parts[1].contains("Debian"))
                {
                    s_vendor = "Debian";
                }
                else if (distribution_parts[1].contains("Pop"))
                {
                    s_vendor = "Pop";
                }
            }

            auto release_parts = release.split(":");
            if (release_parts.size() > 1)
            {
                s_version = Version::version_from_string(release_parts[1]);
            }
        }

#if defined(__GNUC__)
        s_compiler_id.name = "GCC";
        s_compiler_id.version = Version(__GNUC__, __GNUC_MINOR__, 0);
#    if defined(__GNUC_PATCHLEVEL__)
        s_compiler_id.version.release = __GNUC_PATCHLEVEL__;
#    endif
#elif defined(__clang__)
        s_compiler_id.name = "Clang";
        s_compiler_id.version = Version(__clang_major__, __clang_minor__, __clang_patchlevel__);
#endif

        s_number_of_processors = static_cast<decltype(s_number_of_processors)>(get_nprocs_conf());
    }
} // namespace TF::Foundation
