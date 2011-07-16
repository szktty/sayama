from optparse import OptionParser
import unittest

import utils
import test_memory

USAGE = '%prog lib [suite...]'
DESC = """
Suites are: all (all suites), memory.
""".strip()

p = OptionParser(USAGE, description=DESC)

if __name__ == '__main__':
    opts, args = p.parse_args()
    if len(args) == 0:
        p.print_help()
        exit(1)
    lib = args.pop(0)
    utils.load(lib)

    all = False
    if len(args) == 0 or 'all' in args:
        all = True

    mods = []
    if all or '' in args:
        mods.extend([test_memory])

    res = unittest.TestResult()
    suite = unittest.TestSuite()
    for mod in mods:
        suite.addTest(mod.test_suite())
    unittest.TextTestRunner(verbosity=2).run(suite)

