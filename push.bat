:: cd /d "%~dp0"

   git add -Af && git commit --allow-empty-message -m ""
   git push origin HEAD:master %*
:: git checkout master --

