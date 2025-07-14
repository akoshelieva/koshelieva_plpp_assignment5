import subprocess
def run_echo(input_data):
 process = subprocess.Popen(
 ['cmake-build-debug/interpreter.exe'],
 stdin=subprocess.PIPE,
 stdout=subprocess.PIPE,
 stderr=subprocess.PIPE,
 text=True
 )
 stdout, stderr = process.communicate(input_data)
 return stdout.strip(), stderr.strip()
def test_echo():
 tests = [
  {
   "input": "-1234\n",
   "expected_output": "-1234"
  },
  {
   "input": "2+3\n",
   "expected_output": "5"
  },
  {
   "input": "-5+2\n",
   "expected_output": "-3"
  },
  {
   "input": "-5--5\n",
   "expected_output": "0"
  },
  {
   "input": "2+3*4\n",
   "expected_output": "14"
  },
  {
   "input": "(2+3)*4\n",
   "expected_output": "20"
  },
  {
   "input": "abs(-3)\n",
   "expected_output": "3"
  },
  {
   "input": "pow(2,10)\n",
   "expected_output": "1024"
  },
  {
   "input": "max(-5,-3)\n",
   "expected_output": "-3"
  },
  {
   "input": "min(4,7)\n",
   "expected_output": "4"
  },
  {
   "input": "var a = 3\n",
   "expected_output": "3"
  },
  {
   "input": "var a = 3\na + a\n",
   "expected_output": "3\n6"
  },
  {
   "input": "var b = 2+5*2\n",
   "expected_output": "12"
  },
  {
   "input": "var b = 2+5*2\nb-4\n",
   "expected_output": "12\n8"
  },
  {
   "input": "def inc(x){x+1}\n",
   "expected_output": "0"
  },
  {
   "input": "def inc(x){x+1}\ninc(10)\n",
   "expected_output": "0\n11"
  },
  {
   "input": "def muladd(a,b){a*a+b}\n",
   "expected_output": "0"
  },
  {
   "input": "def muladd(a,b){a*a+b}\nmuladd(3,4)\n",
   "expected_output": "0\n13"
  },
  {
   "input": "pow(2, max(3,2))\n",
   "expected_output": "8"
  },
  {
   "input": "pow(-2,2)\n",
   "expected_output": "4"
  }
 ]

 for test in tests:
  input_data = test["input"]
  expected_output = test["expected_output"]

  output, error = run_echo(input_data)

  if error:
   print(f"Test failed for input:\n{input_data}")
   print(f"Error:\n{error}")
  elif output.strip() != expected_output:
   print(f"Test failed for input:\n{input_data}")
   print(f"Expected:\n{expected_output}")
   print(f"Got:\n{output}")
  else:
   print(f"Test passed for input:\n{input_data}")
if __name__ == "__main__":
 test_echo()
