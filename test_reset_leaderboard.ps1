# Test Script für Leaderboard Reset Funktionalität
# Stelle sicher, dass der Server läuft: start_server.bat

Write-Host "`n========================================" -ForegroundColor Cyan
Write-Host "  LEADERBOARD RESET TEST" -ForegroundColor Cyan
Write-Host "========================================`n" -ForegroundColor Cyan

$baseUrl = "http://127.0.0.1:5000"

# Test 1: Einzelnes Leaderboard zurücksetzen (Heißer Draht)
Write-Host "Test 1: Heißer Draht Leaderboard zurücksetzen" -ForegroundColor Yellow
Write-Host "-----------------------------------------------" -ForegroundColor Yellow
try {
    $response = Invoke-WebRequest -Uri "$baseUrl/admin/reset_leaderboard" -Method POST -Body @{game_type='heisser_draht'} -UseBasicParsing
    $result = $response.Content | ConvertFrom-Json
    
    if ($result.success) {
        Write-Host "✅ SUCCESS: $($result.message)" -ForegroundColor Green
        Write-Host "   Backup: $($result.backup_file)" -ForegroundColor Gray
    } else {
        Write-Host "❌ FAILED: $($result.message)" -ForegroundColor Red
    }
} catch {
    Write-Host "❌ ERROR: $($_.Exception.Message)" -ForegroundColor Red
}

Write-Host ""

# Test 2: Einzelnes Leaderboard zurücksetzen (Vier Gewinnt)
Write-Host "Test 2: Vier Gewinnt Leaderboard zurücksetzen" -ForegroundColor Yellow
Write-Host "-----------------------------------------------" -ForegroundColor Yellow
try {
    $response = Invoke-WebRequest -Uri "$baseUrl/admin/reset_leaderboard" -Method POST -Body @{game_type='vier_gewinnt'} -UseBasicParsing
    $result = $response.Content | ConvertFrom-Json
    
    if ($result.success) {
        Write-Host "✅ SUCCESS: $($result.message)" -ForegroundColor Green
        Write-Host "   Backup: $($result.backup_file)" -ForegroundColor Gray
    } else {
        Write-Host "❌ FAILED: $($result.message)" -ForegroundColor Red
    }
} catch {
    Write-Host "❌ ERROR: $($_.Exception.Message)" -ForegroundColor Red
}

Write-Host ""

# Test 3: Einzelnes Leaderboard zurücksetzen (Puzzle)
Write-Host "Test 3: Puzzle Leaderboard zurücksetzen" -ForegroundColor Yellow
Write-Host "-----------------------------------------------" -ForegroundColor Yellow
try {
    $response = Invoke-WebRequest -Uri "$baseUrl/admin/reset_leaderboard" -Method POST -Body @{game_type='puzzle'} -UseBasicParsing
    $result = $response.Content | ConvertFrom-Json
    
    if ($result.success) {
        Write-Host "✅ SUCCESS: $($result.message)" -ForegroundColor Green
        Write-Host "   Backup: $($result.backup_file)" -ForegroundColor Gray
    } else {
        Write-Host "❌ FAILED: $($result.message)" -ForegroundColor Red
    }
} catch {
    Write-Host "❌ ERROR: $($_.Exception.Message)" -ForegroundColor Red
}

Write-Host ""

# Test 4: Alle Leaderboards zurücksetzen (VORSICHT!)
Write-Host "Test 4: ALLE Leaderboards zurücksetzen" -ForegroundColor Yellow
Write-Host "-----------------------------------------------" -ForegroundColor Yellow
Write-Host "⚠️  WARNUNG: Dies löscht ALLE Spieldaten!" -ForegroundColor Red
Write-Host "    (Nur für Testzwecke - normalerweise nicht empfohlen)" -ForegroundColor Gray

$confirm = Read-Host "`nWirklich ALLE Leaderboards zurücksetzen? (yes/no)"

if ($confirm -eq "yes") {
    try {
        $response = Invoke-WebRequest -Uri "$baseUrl/admin/reset_leaderboard" -Method POST -Body @{game_type='all'} -UseBasicParsing
        $result = $response.Content | ConvertFrom-Json
        
        if ($result.success) {
            Write-Host "✅ SUCCESS: $($result.message)" -ForegroundColor Green
            Write-Host "   Backup: $($result.backup_file)" -ForegroundColor Gray
        } else {
            Write-Host "❌ FAILED: $($result.message)" -ForegroundColor Red
        }
    } catch {
        Write-Host "❌ ERROR: $($_.Exception.Message)" -ForegroundColor Red
    }
} else {
    Write-Host "⏭️  Test übersprungen" -ForegroundColor Gray
}

Write-Host ""

# Test 5: Ungültiger Game Type
Write-Host "Test 5: Ungültigen Game Type testen" -ForegroundColor Yellow
Write-Host "-----------------------------------------------" -ForegroundColor Yellow
try {
    $response = Invoke-WebRequest -Uri "$baseUrl/admin/reset_leaderboard" -Method POST -Body @{game_type='invalid_game'} -UseBasicParsing -ErrorAction Stop
    Write-Host "❌ FAILED: Sollte einen Fehler zurückgeben" -ForegroundColor Red
} catch {
    Write-Host "✅ SUCCESS: Fehler erkannt (erwartetes Verhalten)" -ForegroundColor Green
    Write-Host "   Fehlermeldung: $($_.Exception.Message)" -ForegroundColor Gray
}

Write-Host ""

# Test 6: Fehlender Game Type
Write-Host "Test 6: Fehlenden Game Type testen" -ForegroundColor Yellow
Write-Host "-----------------------------------------------" -ForegroundColor Yellow
try {
    $response = Invoke-WebRequest -Uri "$baseUrl/admin/reset_leaderboard" -Method POST -Body @{} -UseBasicParsing -ErrorAction Stop
    Write-Host "❌ FAILED: Sollte einen Fehler zurückgeben" -ForegroundColor Red
} catch {
    Write-Host "✅ SUCCESS: Fehler erkannt (erwartetes Verhalten)" -ForegroundColor Green
    Write-Host "   Fehlermeldung: $($_.Exception.Message)" -ForegroundColor Gray
}

Write-Host ""
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  TEST ABGESCHLOSSEN" -ForegroundColor Cyan
Write-Host "========================================`n" -ForegroundColor Cyan

Write-Host "💡 Hinweise:" -ForegroundColor Yellow
Write-Host "   - Backup-Dateien wurden erstellt (leaderboard_backup_*.json)" -ForegroundColor Gray
Write-Host "   - Öffne Admin-Panel: $baseUrl/admin" -ForegroundColor Gray
Write-Host "   - Überprüfe Leaderboards: $baseUrl/" -ForegroundColor Gray
