const std = @import("std");

pub fn build(b: *std.Build) void {
    const exe = b.addExecutable(.{
        .name = "gen",
        .target = b.graph.host,
    });

    const c_src_files: []const []const u8 = &.{
        "main.cpp",
        "utils.cpp",
        "find-constant.cpp",
        "tests.cpp",
        "test_files.cpp",
    };

    exe.addCSourceFiles(.{
        .root = .{ .cwd_relative = "src" },
        .files = c_src_files,
    });

    exe.linkLibCpp();
    exe.linkSystemLibrary("z3");
    b.installArtifact(exe);

    const run_exe = b.addRunArtifact(exe);
    const run_step = b.step("run", "run produced executable.");
    run_step.dependOn(&run_exe.step);
}
