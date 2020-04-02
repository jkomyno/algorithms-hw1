# Usage: ./time.ps1 [program-name] > outputs.csv
# create a UTF-16 encoded CSV file that can be easily processed by Python

echo "timing;output"

$program = $args[0]
$prefix = $args[1]

Get-ChildItem "./" -Filter "$prefix*.txt" | 
Foreach-Object {
    $filename = $_.FullName

    $timing = Measure-Command {
      $output = (Get-Content $filename | & $program)
    } | Select -ExpandProperty Milliseconds

    echo "$timing;$output"
}
