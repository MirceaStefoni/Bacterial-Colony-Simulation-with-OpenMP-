
$program = "C:\Users\Mircea\Desktop\APD\Assignment_2\print_data_version.exe"  # Name of the program
$arg1 = "C:\Users\Mircea\Desktop\APD\Assignment_2\bacteria5000.txt"    # Input file                  
$arg2 = "10"      # Generations                
$arg3 = "4"       # Threads               

# Loop to run the program 25 times
for ($i = 1; $i -le 25; $i++) {
    Write-Host "Running iteration $i..."
    Start-Process $program -ArgumentList $arg1, $arg2, $arg3 -Wait
}

Write-Host "Finished running the program 25 times."

# Run with ./run_program.ps1