[void] [System.Reflection.Assembly]::LoadWithPartialName("System.Windows.Forms")
Start-Process powershell.exe -ArgumentList "-file ${env:HOME}\.github\workflows\runcraft.ps1" -NoNewWindow

Start-Sleep -m 3000

[System.Windows.Forms.SendKeys]::SendWait("${env:HOME}\Craft64")
[System.Windows.Forms.SendKeys]::SendWait("{ENTER}")

Start-Sleep -m 20000

[System.Windows.Forms.SendKeys]::SendWait("0")
[System.Windows.Forms.SendKeys]::SendWait("{ENTER}")

Start-Sleep -m 1000
[System.Windows.Forms.SendKeys]::SendWait("{ENTER}")

Start-Sleep -m 1000
[System.Windows.Forms.SendKeys]::SendWait("{ENTER}")

Start-Sleep -m 1000
[System.Windows.Forms.SendKeys]::SendWait("{ENTER}")

Start-Sleep -m 1000
[System.Windows.Forms.SendKeys]::SendWait("{ENTER}")