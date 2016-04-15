from setuptools import setup

setup(
    name = 'parabench',
    version = '0.0.1',
    description = ' Drive MPI aparallel bencmark run',
    url = 'https://github.com/dfroger/parabench',
    packages = ['parabench',],
    entry_points = {
        'console_scripts': [
            'parabench = parabench.parabench:main',
        ],
    },
    license = 'BSD',
    author = 'David Froger',
    author_email = 'david.froger@mailoo.org',
)

