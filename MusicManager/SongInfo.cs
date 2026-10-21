class Artist
{
    public string id { get; set; }
    public string name { get; set; } = "";

    public Dictionary<int, Song> Songs { get; set; } = new();
}

class Song
{
    public string id { get; set; }
    public string name { get; set; } = "";
    public string path { get; set; } = "";
}
