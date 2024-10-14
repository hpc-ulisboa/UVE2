const fs = require('fs');
const path = require('path');

const fileExtensions = ['.txt', '.dump', '.s', '.ll', '.o', '.din', ''];

function deleteLogsAndExecutables(dirPath) {
    fs.readdir(dirPath, { withFileTypes: true }, (err, files) => {
        if (err) throw err;

        files.forEach(file => {
            const fullPath = path.join(dirPath, file.name);

            if (file.isDirectory()) {
                // Recurse into subdirectories
                deleteLogsAndExecutables(fullPath);
            } else if (file.isFile()) {
                // Delete .txt files and executables
                if (fileExtensions.includes(path.extname(file.name))) {
                    fs.unlink(fullPath, err => {
                        if (err) throw err;
                        console.log(`Deleted ${fullPath}`);
                    });
                }
            }
        });

        // After all files in the directory have been handled, check if the directory is empty
        fs.readdir(dirPath, (err, files) => {
            if (err) throw err;

            if (files.length === 0) {
                // If the directory is empty, delete it
                fs.rmdir(dirPath, err => {
                    if (err) throw err;
                    console.log(`Deleted empty directory ${dirPath}`);
                });
            }
        });
    });
}

deleteLogsAndExecutables('benchmarks');
