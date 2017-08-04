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

class WafBuildCommand(WafCommand):
    def run(self):
        subprocess.check_call(
            ["./waf", "--top=snn", "--out=build", "build"])
        for f in glob.glob("build/python/*.so"):
            shutil.copy(f, ".")

class BuildExt(setuptools.command.build_ext.build_ext):
    def run(self):
        self.run_command("waf_configure")
        self.run_command("waf_build")

setup(
    name="snn",
    cmdclass={
        "waf_configure": WafConfigureCommand,
        "waf_build": WafBuildCommand,
        "build_ext": BuildExt,
    },
)
