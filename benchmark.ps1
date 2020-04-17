# Usage: ./benchmark.ps1 {dataset-folder}

$exe_folder = ".\x64\Release"
$programs = "PrimBinaryHeap","PrimKHeap","KruskalUnionFind","KruskalUnionFindCompressed","KruskalNaive"
$output_folder = ".\benchmark"

$datasets = "datasets"
if($args[0] -ne $null) {
    $datasets = $args[0]
}

# necessary to ensure all [io.file] methods are relative to this directory instead
# of "C:\Windows\System 32"
[System.IO.Directory]::SetCurrentDirectory($PWD)

foreach ($program in $programs) {
  $timestamp = $(get-date -f "yyyyMMdd_HHmmss_ff")
  $output_filename = "$program.$timestamp.csv"
  $destination = "$output_folder\$output_filename"

  echo "Benchmark start: $program"

  # [io.file] methods are used to avoid newlines on append.
  # The first write has to be performed with [io.file] too:
  # using echo would result in corrupt characters in [io.file]::AppendAllText.
  [io.file]::WriteAllText($destination, "ms;output;n;m;filename")
  [io.file]::AppendAllText($destination, [Environment]::NewLine)

  Get-ChildItem "./" -Filter "$datasets/*.txt" |
  Foreach-Object {
    $filename = $_.FullName
    $basename = $_.Basename
    echo "$basename.txt"
    (& .\time.ps1 "$exe_folder\$program" $filename) | Out-String | Tee-Object -variable out
    [io.file]::AppendAllText($destination, $out)
  }

  echo "Benchmark end: $program"
  echo ""
}
