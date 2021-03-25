"""    
    Utility to inspect FBX files.

    2021-∞ (c) blurryroots innovation qanat OÜ. All rights reserved.
    See license.md for details.

    https://think-biq.com
"""

import setuptools
import os
from src import version

PROJECT_PATH = os.path.dirname(os.path.realpath(__file__))

with open(os.path.join(PROJECT_PATH, 'readme.md'), 'r') as fh:
    long_description = fh.read()

setuptools.setup(
    python_requires='>=3.9',
    name="wandel",
    version=version(),
    description="Utility to inspect FBX files.",
    long_description=long_description,
    long_description_content_type="text/markdown",
    package_dir = {'wandel': 'src', 'wandel_cli': 'src/wandel_cli'},
    packages=['wandel', 'wandel_cli'],
    include_package_data=True,    
    package_data={'wandel_cli': ['pywandel.*.pyd', 'pywandel.*.so']},
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: MIT License",
        "Operating System :: OS Independent",
    ],
    author="biq",
    author_email="sf@think-biq.com",
    url="https://gitlab.com/think-biq/Verwandlung"
)