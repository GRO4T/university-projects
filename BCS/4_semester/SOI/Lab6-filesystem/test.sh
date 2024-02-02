echo "TEST 1 INTERNAL FRAGMENTATION"
echo "Creating virtual filesystem..."
./VFilesystem -f testDrive -c 16
./VFilesystem -f testDrive -M
echo ""
echo "Copying files to the filesystem..."

./VFilesystem -f testDrive -u files/file1_5kB -n a
./VFilesystem -f testDrive -u files/file1_5kB -n b
./VFilesystem -f testDrive -u files/file1_5kB -n c
./VFilesystem -f testDrive -u files/file1_5kB -n d

echo ""
./VFilesystem -f testDrive -l
./VFilesystem -f testDrive -D

echo "Uploading another file"
echo ""

./VFilesystem -f testDrive -u files/file1_5kB -n e

echo ""
echo "TEST 2 EXTERNAL FRAGMENTATION"

echo "Creating virtual filesystem..."
./VFilesystem -f testDrive -c 12
./VFilesystem -f testDrive -M

echo ""
echo "Copying files to the filesystem..."

./VFilesystem -f testDrive -u files/file1kB -n a
./VFilesystem -f testDrive -u files/file1kB -n b
./VFilesystem -f testDrive -u files/file1kB -n c
./VFilesystem -f testDrive -u files/file1kB -n d


echo ""
./VFilesystem -f testDrive -l
./VFilesystem -f testDrive -D

echo ""
echo "Deleting files from the filesystem"
echo "" 

./VFilesystem -f testDrive -r b
./VFilesystem -f testDrive -r d
./VFilesystem -f testDrive -D

echo ""
echo "Adding 2kB file"
echo ""

./VFilesystem -f testDrive -u files/file2kB -n e
./VFilesystem -f testDrive -D

echo ""
echo "Deleting the filesystem..."
