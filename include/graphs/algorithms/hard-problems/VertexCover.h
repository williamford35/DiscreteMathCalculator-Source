#define maxn 25

class VertexCover
{
public:
	VertexCover ();
	~VertexCover();
	int findVertexCover(int n, int m);

private:
	bool gr[maxn][maxn];

	// Returns true if there is a possible subset
	// of size 'k' that can be a vertex cover
	bool isCover(int V, int k, int E);
	// Inserts an edge in the graph
	void insertEdge(int u, int v);
};
