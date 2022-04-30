<?php
require "linux_config.php";

$clang .= " -fuse-ld=";
if(shell_exec("which lld"))
{
	$clang .= "l";
}
$clang .= "ld -lstdc++ -lstdc++fs -pthread -lresolv -lm";

passthru("$clang \"CLI/cli.cpp\" \"CLI/cli_3d.cpp\" \"CLI/cli_repl.cpp\" libsoup.a -I soup -o \"soupcli\"");
