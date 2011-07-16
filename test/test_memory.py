from ctypes import *
import unittest
import utils

class TestMemory(unittest.TestCase):

    def setUp(self):
        pass

    def testmemset(self):
        self.assert_(True)
        buf = c_char_p('\0' * 128)
        utils.lib.sy_memset(buf, c_int(1), c_size_t(128))
        print len(buf.value)
        print buf.value[0] == '\1'

def test_suite():
    return utils.test_suite([TestMemory])

