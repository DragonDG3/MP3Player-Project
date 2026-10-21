namespace SDCardDFPlayerFormater;

using System.Text.Json;

class Program
{
    static Dictionary<int, Artist> artists = new();
    static string[] folders;
    static string[] files;
    static DriveManager drive = new();
    const string JSONPATH = "../src/musiclist.json";
    static readonly JsonSerializerOptions jsonOptions = new() { WriteIndented = true };
    static void Main(string[] args)
    {
        Console.Clear();
        drive.DriveSetup();

        // ReadFolders();
        // SaveJSON();
        // RenameFolders(drive.getPath);

        LoadJSON();

        foreach (var artist in artists)
        {
            Console.WriteLine($"{artist.Key} - \"{artist.Value.id}\": {artist.Value.name}");
            foreach (var song in artist.Value.Songs)
            {
                Console.WriteLine($"    {song.Key} - \"{song.Value.id}\": {song.Value.name}");
            }
        }
    }
    static void SaveJSON()
    {
        if (!File.Exists(JSONPATH))
            File.Create(JSONPATH);
        File.WriteAllText(JSONPATH, JsonSerializer.Serialize(artists, jsonOptions));

        Console.WriteLine($"Saved {artists.Count} folders to {JSONPATH}");
    }
    static void LoadJSON()
    {
        if (!File.Exists(JSONPATH)) return;

        string text = File.ReadAllText(JSONPATH);
        if (string.IsNullOrWhiteSpace(text)) return;

        artists = JsonSerializer.Deserialize<Dictionary<int, Artist>>(text, jsonOptions);
    }

    static void RenameFolders(string root)
    {
        string[] dirs = Directory.GetDirectories(root)
            .Where(d => !Path.GetFileName(d)
                    .StartsWith('.'))
            .OrderBy(d => d, StringComparer.OrdinalIgnoreCase)
            .ToArray();

        var tempDirs = new List<string>();
        foreach (var dir in dirs) // First pass is to rename to temporary names just in case there are folders with 000 names
        {
            string temp = Path.Combine(root, "tmp_" + Guid.NewGuid().ToString("N"));
            Directory.Move(dir, temp);
            tempDirs.Add(temp);
        }

        for (int i = 0; i < tempDirs.Count; i++) // Second pass renames them to the real names
        {
            string dir = Path.Combine(root, $"{i + 1:D2}");
            Directory.Move(tempDirs[i], dir);
            RenameFiles(dir);
        }
    }
    static void RenameFiles(string dir)
    {
        string[] files = Directory.GetFiles(dir)
            .OrderBy(f => f, StringComparer.OrdinalIgnoreCase)
            .ToArray();

        List<string> tempFiles = new();
        foreach (var file in files)
        {
            string temp = Path.Combine(dir, "tmp_" + Guid.NewGuid().ToString("N") + Path.GetExtension(file));
            Directory.Move(file, temp);
            tempFiles.Add(temp);
        }

        for (int i = 0; i < tempFiles.Count; i++)
        {
            string extension = Path.GetExtension(tempFiles[i]);
            string file = Path.Combine(dir, $"{i + 1:D3}{extension}");
            Directory.Move(tempFiles[i], file);
        }
    }

    static void ReadFolders()
    {
        folders = Directory.GetDirectories(drive.getPath)
            .Where(folder => !Path.GetFileName(folder)
                    .StartsWith('.'))
            .OrderBy(folder => folder, StringComparer.OrdinalIgnoreCase)
            .ToArray();

        for (int i = 0; i < folders.Length; i++)
        {
            artists.Add(i, new Artist
            {
                id = $"{i + 1:D2}",
                name = Path.GetFileName(folders[i])
            });

            // Console.WriteLine($"{i}: [DIR] {Path.GetFileName(folders[i])}");

            ReadFiles(Path.Combine(drive.getPath, Path.GetFileName(folders[i])), i);
        }
    }
    static void ReadFiles(string path, int artistNumber)
    {
        files = Directory.GetFiles(path)
            .OrderBy(f => f, StringComparer.OrdinalIgnoreCase)
            .ToArray();

        for (int i = 0; i < files.Length; i++)
        {
            artists[artistNumber].Songs.Add(i, new Song
            {
                id = $"{i + 1:D3}",
                name = Path.GetFileName(files[i]),
                path = path
            });

            // Console.WriteLine($"    {i}: [FILE] {Path.GetFileName(files[i])}");

        }
    }
}
