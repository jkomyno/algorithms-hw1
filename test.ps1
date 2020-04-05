# Usage: ./test.ps1 [program-name]
# Execute [program-name] against all the input files in test/ and compares the result
# of the program with the corresponding output file

$program = $args[0]

Get-ChildItem "./" -Filter test/input_random*.txt |
Foreach-Object {
    $input_file = $_.FullName
    $basename = $_.Basename

    $Null = $input_file -match '(.*)input_random(?<Id>.*)'
    $id = $Matches.Id
    $output_file = "test/output_random$id"

    $mst_weight = Get-Content $output_file

    $cmd_output = (Get-Content $input_file | & $program)

    If (-NOT ($mst_weight -eq $cmd_output)) {
      throw "MST mismatch on input $basename.txt. Got $cmd_output, wanted $mst_weight."
    } else {
      echo "OK $basename.txt"
    }
}
