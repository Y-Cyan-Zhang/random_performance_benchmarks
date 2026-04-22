# Simple script that compiles for different compiler optimization levels.
import subprocess
import os

source = "push_back_vs_indexing.cpp"
flags = ["-O0", "-O1", "-O2", "-O3"]

for flag in flags:
    output = f"test{flag}"
    
    # Compile
    compile_cmd = ["g++", flag, source, "-o", output]
    subprocess.run(compile_cmd, check=True)
    
    # Run and capture output
    result = subprocess.run([f"./{output}"], capture_output=True, text=True)
    
    print(f"=== {flag} ===")
    print(result.stdout)
    
    # Cleanup
    os.remove(output)
