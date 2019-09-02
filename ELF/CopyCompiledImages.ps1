$temp_path = (Join-Path -Path $env:LOCALAPPDATA -ChildPath "Temp")
$files = Get-Childitem –Path $temp_path -Include KimchiConLogo.ino.elf,KimchiConLogo.ino.bin,KimchiConLogo.ino.partitions.bin -File -Recurse -ErrorAction SilentlyContinue

$target_folder = "Images"
New-Item -ItemType Directory -Force -Path $target_folder

foreach ($file in $files)
{
	Write-Host "Copying file " $file " to " $target_folder
	Copy-Item $file -Destination $target_folder	
}