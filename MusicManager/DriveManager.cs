public class DriveManager
{
    private string PATH;

    public void DriveSetup()
    {
        Console.WriteLine();
        DriveInfo[] drives = DriveInfo.GetDrives()
            .Where(drive => drive.IsReady && drive.DriveType == DriveType.Removable)
            .ToArray();

        if (drives.Length == 0) { Console.WriteLine("No drives found."); return; }
        DriveInfo drive = drives.Length > 1
            ? SelectDrive(drives)
            : drives[0];
        if (drive == null) { Console.WriteLine("No available drives."); return; }

        PATH = Path.Combine(drive.RootDirectory.FullName);

        Console.WriteLine($"Drive selected: {drive.Name}");
        Console.WriteLine($"Music path: {PATH}");
    }
    private DriveInfo SelectDrive(DriveInfo[] drives)
    {
        while (true)
        {
            Console.Clear();
            for (int i = 0; i < drives.Length; i++)
                Console.WriteLine($"{i}: {drives[i].Name} ({drives[i].VolumeLabel})");

            Console.WriteLine("Select the SD Card");
            if (!int.TryParse(Console.ReadLine(), out int choice) ||
                    choice >= 0 || choice < drives.Length)
                return drives[choice];

            Console.WriteLine("Invalid drive. Press any key to try again");
            Console.ReadKey();

        }
    }

    public string getPath => PATH;
}
