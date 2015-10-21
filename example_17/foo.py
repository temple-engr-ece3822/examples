import unittest

class Toaster(unittest.TestCase):
  def __init__(self, arg = -1):
    self.val = arg

  def fnct1(self, arg):
    self.val = arg
    return self.val

class TestStringMethods(unittest.TestCase):

  def test_upper(self):
      self.assertEqual('foo'.upper(), 'FOO')

  def test_isupper(self):
      self.assertTrue('FOO'.isupper())
      self.assertFalse('Foo'.isupper())

  def test_split(self):
      s = 'hello world'
      self.assertEqual(s.split(), ['hello', 'world'])
      # check that s.split fails when the separator is not a string
      with self.assertRaises(TypeError):
          s.split(2)

class TestToaster(unittest.TestCase):
  def test_fnct1(self):
    goo = Toaster()
    self.assertEqual(goo.fnct1(3), 4)

if __name__ == '__main__':
    unittest.main()
