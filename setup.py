import distutils.cmd
import glob
from setuptools import setup
import setuptools.command.build_ext
import shutil
import subprocess

class WafCommand(distutils.cmd.Command):
    user_options=[]
    def initialize_options(self):
        pass
    def finalize_options(self):
        pass

class WafConfigureCommand(WafCommand):
    def run(self):
        subprocess.check_call(
            ["./waf", "--top=snn", "--out=build", "configure"])

class WafBuildDebugCommand(WafCommand):
    def run(self):
        subprocess.check_call(
            ["./waf", "--top=snn", "--out=build", "debug"])
        for f in glob.glob("build/debug/python/*.so"):
            shutil.copy(f, ".")

class WafBuildReleaseCommand(WafCommand):
    def run(self):
        subprocess.check_call(
            ["./waf", "--top=snn", "--out=build", "release"])
        for f in glob.glob("build/release/python/*.so"):
            shutil.copy(f, ".")

class BuildExt(setuptools.command.build_ext.build_ext):
    def run(self):
        self.run_command("waf_configure")
        self.run_command("waf_build_release")

setup(
    name="snn",
    cmdclass={
        "waf_configure": WafConfigureCommand,
        "waf_build_debug": WafBuildDebugCommand,
        "waf_build_release": WafBuildReleaseCommand,
        "build_ext": BuildExt,
    },
)
