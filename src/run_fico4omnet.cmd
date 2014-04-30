@echo off
if exist %~dp0\FiCo4OMNeT.exe (
    %~dp0\FiCo4OMNeT -n %~dp0\..\examples;%~dp0 %*
) else (
    opp_run -l %~dp0\FiCo4OMNeT -n %~dp0\..\examples;%~dp0 %*
)
