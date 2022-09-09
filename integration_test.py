import pexpect
import pexpect.replwrap as replwrap
import unittest
import os
	
# the slisp executable
cmd = './slisp'

# the prompt to expect
prompt = u'slisp>'

class TestREPL(unittest.TestCase):

	def setUp(self):
		self.wrapper = replwrap.REPLWrapper(cmd, prompt, None)
		
	def test_add(self):
		output = self.wrapper.run_command(u'(+ 1 2)')
		self.assertEqual(output.strip(), "(3)")
		output = self.wrapper.run_command(u'(+ 1 2 10)')
		self.assertEqual(output.strip(), "(13)")
		output = self.wrapper.run_command(u'(+ 1 2 0)')
		self.assertEqual(output.strip(), "(3)")
		output = self.wrapper.run_command(u'(+ 0 1 2)')
		self.assertEqual(output.strip(), "(3)")
		output = self.wrapper.run_command(u'(+ 1 2 -2)')
		self.assertEqual(output.strip(), "(1)")
		output = self.wrapper.run_command(u'(+ -1 -2)')
		self.assertEqual(output.strip(), "(-3)")

	def test_define(self):
		output = self.wrapper.run_command(u'(define a True)')
		self.assertEqual(output.strip(), "(True)")
		output = self.wrapper.run_command(u'(define b a)')
		self.assertEqual(output.strip(), "(True)")
		output = self.wrapper.run_command(u'(and a b)')
		self.assertEqual(output.strip(), "(True)")

	def test_conditional(self):
		self.wrapper.run_command(u'(define a -1)')
		self.wrapper.run_command(u'(define b 1)')
		self.wrapper.run_command(u'(define c 0)')
		output = self.wrapper.run_command(u'(if (< a b) c False)')
		self.assertEqual(output.strip(), "(0)")

	def test_error(self):
		output = self.wrapper.run_command(u'(define begin True)')
		self.assertTrue(output.strip().startswith('Error'))
				
class TestExecuteCommandline(unittest.TestCase):
		
	def test_sub(self):
		args = ' -e ' + ' "(- 4 2)" '
		(output, retcode) = pexpect.run(cmd+args, withexitstatus=True, extra_args=args)
		self.assertEqual(retcode, 0)
		self.assertEqual(output.strip(), b"(2)")

	def test_error(self):
		args = ' -e ' + ' "(- 4 2 12)" '
		(output, retcode) = pexpect.run(cmd+args, withexitstatus=True, extra_args=args)
		self.assertNotEqual(retcode, 0)
		self.assertTrue(output.strip().startswith(b'Error'))

class TestExecuteFromFile(unittest.TestCase):
		
	def test_sub(self):
		args = ' /vagrant/tests/test3.slp'
		(output, retcode) = pexpect.run(cmd+args, withexitstatus=True, extra_args=args)
		self.assertEqual(retcode, 0)
		self.assertEqual(output.strip(), b"(2)")

	def test_error(self):
		args = ' /there/is/no/such/file'
		(output, retcode) = pexpect.run(cmd+args, withexitstatus=True, extra_args=args)
		self.assertNotEqual(retcode, 0)
		self.assertTrue(output.strip().startswith(b'Error'))

# run the tests
unittest.main()
