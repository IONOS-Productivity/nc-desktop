if ($IsWindows) {
    $python = (Get-Command py).Source
    $python=(py -c "import sys; print(sys.executable)")
} else {
    $python = (Get-Command python3).Source
}

$command = @("${env:HOME}\craft\CraftMaster\CraftMaster.py",
             "--config", "${env:HOME}\craftmaster.ini",
             "--target", "${env:CRAFT_TARGET}") + $args

Write-Host "Exec: ${python} ${command}"

& $python @command
if ($LASTEXITCODE -ne 0) {
    exit 1
}