# Usage: ./testall.ps1

$exe_folder = ".\x64\Release"
$programs = "PrimBinaryHeap","KruskalUnionFind","KruskalUnionFindCompressed","KruskalNaive"

foreach ($program in $programs) {
  echo "Testing $program"
  & { Invoke-Expression ".\test.ps1 $exe_folder\$program" } 2>&1 | Tee-Object -Variable out_content
  echo ""
}

# Run tests in parallel
# $programs | ForEach-Object {
#   Start-Job -Init ([ScriptBlock]::Create("Set-Location '$pwd'")) -FilePath .\test.ps1 -ArgumentList "$exe_folder\$program"
# } | Wait-Job | Receive-Job
