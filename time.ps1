# Usage: ./time.ps1 [program-name] [dataset-prefix] > outputs.csv
# create a UTF-16 encoded CSV file that can be easily processed by Python
#
# The CSV file has the following columns
# ms: number of milliseconds needed to execute the algorithm on a single file
# output: weight of the MST that was found
# n: number of graph nodes
# m: number of graph edges
# filename: name of the input file that contained the graph definition

echo "ms;output;n;m;filename"

$program = $args[0]
$prefix = $args[1]

Get-ChildItem "./" -Filter "$prefix*.txt" |
Foreach-Object {
    $filename = $_.FullName
    $basename = $_.Basename

    $first_line = Get-Content $filename -First 1
    $n,$m = $first_line.split(' ')

    $ms = Measure-Command {
      $output = (Get-Content $filename | & $program)
    } | Select -ExpandProperty Milliseconds

    echo "$ms;$output;$n;$m;$basename"
}
