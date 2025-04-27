from skbuild import setup

setup(
    name="PyMCS",
    version="0.1.0",
    description="A library for interfacing with SmarAct MCS controllers.",
    author="Cameron Devine",
    author_email="cameron.devine@alleninstitute.org",
    license="MIT",
    packages=["PyMCS"],
    package_dir={"": "src"},
    cmake_install_dir="src/PyMCS",
    python_requires=">=3.9",
)
