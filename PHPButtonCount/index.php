<?php
function in_arrayi($needle, $haystack)
{
    foreach ($haystack as $value)
    {
        if (strtolower($value) == strtolower($needle))
        {
            return true;
        }
    }
    return false;
}
$types = array('FM2');
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd"> 
<html xmlns="http://www.w3.org/1999/xhtml"> 
<head> 
<title>Minimum Buttons</title> 
<meta name="description" content="Minimum Buttons." /> 
<meta name="keywords" content="minimum, buttons" /> 
<meta name="author" content="Brandon Evans" /> 
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" /> 
</head>
<body>
<?php
if ($_POST['submit'])
{
    $type = explode('.', $_FILES['file']['name']);
    $type = $type[count($type) - 1];
    if (!in_arrayi($type, $types))
    {
?>
<p>The provided file is an unsupported type.</p>
<?php
    }
    else
    {
        $file = file_get_contents($_FILES['file']['tmp_name']);
        $frames = 0;
        $holds = 0;
        $last = array();
        $presses = 0;
        foreach(preg_split("/(\r?\n)/", $file) as $line)
        {
            // This is only a frame if it starts with a vertical bar.
            if ($line[0] == '|')
            {
                $frames++;
                $players = array();
                // Split up the sections by a vertical bar.
                $sections = explode('|', $line);
                foreach ($sections as $index => $section)
                {
                    if ($index > 1 && $index < count($sections) - 2)
                    {
                        if (count($players) < $index - 1)
                        {
                            array_push($players, array());
                        }
                        $split = preg_split(
                            '//', $section, -1, PREG_SPLIT_NO_EMPTY
                        );
                        foreach ($split as $button => $text)
                        {
                            $pressed = false;
                            // Check if this button is pressed.
                            if (!in_array($text, array(' ', '.')))
                            {
                                $holds++;
                                /*
                                If the button was not previously pressed,
                                increment.
                                */
                                if (
                                    count($last) < $index - 1 ||
                                    !$last[$index - 2][$button]
                                )
                                {
                                    $presses++;
                                }
                                // Mark this button as pressed.
                                $pressed = true;
                            }
                            array_push($players[$index - 2], $pressed);
                        }
                    }
                }
                $last = $players;
            }
        }
?>
<p>Frames: <?php echo $frames; ?></p>
<p>Holds: <?php echo $holds; ?></p>
<p>Presses: <?php echo $presses; ?></p>
<?php
    }
}
?>
<p>Supported file types: <?php echo implode(', ', $types); ?></p>
<form enctype="multipart/form-data" action="#" method="POST">
<input name="file" type="file" />
<input name="submit" type="submit" value="Submit" />
</form>
</body>
</html>