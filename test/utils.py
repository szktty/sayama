from ctypes import CDLL
import unittest

lib = None

def load(path):
    global lib
    lib = CDLL(path)

def test_suite(tcs):
    suite = unittest.TestSuite()
    for tc in tcs:
        suite.addTest(unittest.makeSuite(tc))
    return suite

