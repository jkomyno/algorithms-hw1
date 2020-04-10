# Usage: ./time.ps1 [program-name] [input-file] > outputs.csv
# create a UTF-16 encoded CSV file that can be easily processed by Python
#
# The CSV file has the following columns
# ms: number of milliseconds needed to execute the algorithm on a single file
# output: weight of the MST that was found
# n: number of graph nodes
# m: number of graph edges
# filename: name of the input file that contained the graph definition

$program = $args[0]
$input_file = $args[1]

$filename = [System.IO.Path]::GetFileName("$input_file")

$first_line = Get-Content $input_file -First 1
$n,$m = $first_line.split(' ')

$ms = Measure-Command {
  $output = (Get-Content $input_file | & $program)
} | Select -ExpandProperty TotalMilliseconds

echo "$ms;$output;$n;$m;$filename"
