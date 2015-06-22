<?php

/**
 * The class to send files to the SD card in the clock
 */
class ClockFiles
{
    //the filename of this script, don't include it in the list
    private $scriptName = null;
    //the server name
    private $serverName = null;
    //the server port
    private $serverPort = 80;
    //the URL of the script directory
    private $scriptDir = null;


    public function __construct()
    {
        $this->serverName = $_SERVER['SERVER_NAME'];
        $this->serverPort = $_SERVER['SERVER_PORT'];
    	$this->scriptName = __FILE__;
        $this->scriptDir = dirname($_SERVER['SCRIPT_NAME']);
    }

    /**
     * Scans and gets all files in the directory
     *
     * @param string $dir The directory name
     * @return array
     */
    private function scanDirectory($dir) 
    {
        $files = array();
        if (isset($dir) && is_readable($dir)) {        
            try {
                $dir = realpath($dir);
                //get all files including all subdirectories
                $it = new RecursiveIteratorIterator(new RecursiveDirectoryIterator($dir), RecursiveIteratorIterator::CHILD_FIRST);

                foreach ($it as $fileName => $fileInfo) {
                    if ($fileInfo->isFile()) {
                        //skip the script
                        if ($this->scriptName == $fileInfo->getPathName()) {
                            continue;
                        }
                        
                        //the relative filename
                        $fileName = str_replace($dir, '', $fileInfo->getPathName());

                        $file = array(
                            //the relative path
                            'file' => $fileName,
                            //the size in bytes
                            //let's make it a string
                            'size' => (string)$fileInfo->getSize(),
                            //the relative URL of the file
                            'url' => $this->scriptDir . $fileName,
                        );
                        $files[] = $file;
                    }
                }
            }
            catch (Exception $e) {}
        }

        return $files;
    }

    /**
     * Creates a JSON response with all files in the current directory
     */
    public function get()
    {
        //the current directory name
        $currentDir = getcwd();

        $files = $this->scanDirectory($currentDir);

        //the output data
        $data = array(
            'serverName' => $this->serverName,
            'serverPort' => $this->serverPort,
            'files' => $files,
        );

        //JSON encode the response
        header('Content-type: application/json');
        echo json_encode($data);
    }
}

$clockFiles = new ClockFiles();
$clockFiles->get();
