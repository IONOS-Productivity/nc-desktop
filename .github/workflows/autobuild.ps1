Write-Output ${env:HOME}

[void] [System.Reflection.Assembly]::LoadWithPartialName("System.Windows.Forms")
$proc =  Start-Process powershell.exe -ArgumentList "-file ${env:HOME}\.github\workflows\runcraft.ps1" -PassThru

Start-Sleep -m 3000

[System.Windows.Forms.SendKeys]::SendWait("${env:HOME}\Craft64")
[System.Windows.Forms.SendKeys]::SendWait("{ENTER}")

Write-Output "Entered Root Path"

Start-Sleep -m 20000

[System.Windows.Forms.SendKeys]::SendWait("0")
[System.Windows.Forms.SendKeys]::SendWait("{ENTER}")

Write-Output "Chose QT version"

Start-Sleep -m 1000
[System.Windows.Forms.SendKeys]::SendWait("{ENTER}")

Start-Sleep -m 1000
[System.Windows.Forms.SendKeys]::SendWait("{ENTER}")

Start-Sleep -m 1000
[System.Windows.Forms.SendKeys]::SendWait("{ENTER}")

Start-Sleep -m 1000
[System.Windows.Forms.SendKeys]::SendWait("{ENTER}")

Write-Output "Install all"

$proc.WaitForExit()