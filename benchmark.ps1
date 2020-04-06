# Usage: ./benchmark.ps1

$exe_folder = ".\x64\Release"
$programs = "PrimBinaryHeap","KruskalUnionFind","KruskalSimple"
$output_folder = ".\benchmark"

foreach ($program in $programs) {
  $timestamp = $(get-date -f "yyyyMMdd_HHmmss_ff")
  $output_filename = "$program.$timestamp.csv"
  echo "Benchmarking $program"
  (& .\time.ps1 "$exe_folder\$program" "datasets/") | Out-String | Tee-Object -variable out

  echo $out > "$output_folder\$output_filename"
}
