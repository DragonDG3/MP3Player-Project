class Artist
{
    public int id { get; set; }
    public string name { get; set; } = "";

    public Dictionary<int, Album> Albums { get; set; } = new();
}

class Album
{
    public int id { get; set; }
    public string name { get; set; } = "";

    public Dictionary<int, Song> songs { get; set; } = new();
}

class Song
{
    public int id { get; set; }
    public string name { get; set; } = "";
    public string path { get; set; } = "";
}
