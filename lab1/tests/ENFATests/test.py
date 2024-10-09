import subprocess

collection_of_errors = []

# Loop through the test cases
for i in range(1, 34):
    dir = "{:02d}".format(i)
    print(f"Test {dir}")
    
    # Read the input file (test.a)
    with open(f"test{dir}/test.a", "r") as input_file:
        # Run the compiled executable (enfaTest) and pass the input
        process = subprocess.Popen(["./enfaTester"], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
        # Communicate the input to the executable and get the output and error
        output, error = process.communicate(input=input_file.read())

        # Print the captured output and error for debugging purposes
        print("Executable Output:")
        print(output)  # This will print the stdout from the executable

        if error:
            print("Executable Error (stderr):")
            print(error)  # This will print the stderr from the executable if there is any

        # Continue your testing logic to compare the output with expected results
        with open(f"test{dir}/test.b", "r") as expected_output_file:
            expected_output = expected_output_file.read().strip()

            if ("accept" in output and "accept" in expected_output) or ("reject" in output and "reject" in expected_output):
                print("OK")
            else:
                print("FAIL")
                print(f"Difference found in test {dir}")
                collection_of_errors.append(dir)

# After running all tests, report errors if there are any
if collection_of_errors:
    print("\nErrors found in the following tests:")
    for error_dir in collection_of_errors:
        print(f"Test {error_dir}")
else:
    print("\nAll tests passed successfully.")