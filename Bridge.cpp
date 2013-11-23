
#include "Bridge.h"

Bridge::Bridge() {
	nnodes=0;
	nodes=new Node[10];
	nodesArrayLength=10;

	nedges=0;
	edges=new Edge[10];
	edgesArrayLength=10;

	tto=tfrom=0;
	tEdge=0;
}

Bridge::~Bridge() {
	delete [] nodes;
	delete [] edges;
}

void Bridge::addEdge(float width,float fromX, float fromY, float toX, float toY) {

	for(int i=nedges; i--;) {
		if((edges[i].from->b.x == fromX &&
			edges[i].from->b.y == fromY &&
			edges[i].to->b.x == toX &&
			edges[i].to->b.y == toY) ||
			(edges[i].to->b.x == fromX &&
			edges[i].to->b.y == fromY &&
			edges[i].from->b.x == toX &&
			edges[i].from->b.y == toY))
			return;
	}

	Node* n1; Node* n2;
	findOrCreateNode2(&n1,fromX, fromY, &n2, toX, toY);
	addEdge(width,n1,n2);
}

void Bridge::addEdge(float width, Node* n1, Node* n2) {
	if(nedges==edgesArrayLength) {
		Edge* ne = new Edge[nedges+10];
		for(int i=nedges; i--;)
			ne[i]=edges[i];
		delete [] edges;
		edges=ne;
		edgesArrayLength+=10;
	}
//	std::cout<< "\nwas here 1" << std::flush;
	edges[nedges].set(width, n1,n2);
//	std::cout<< "\nwas here 2" << std::flush;
	++nedges;
	float halfWeight=sqrt(pow(n1->p.x-n2->p.x,2)+pow(n1->p.y-n2->p.y,2))/2.0f;
//	std::cout<< "\nwas here 3 " << n1 << "  " << n2 << std::flush;
	n1->weight+=halfWeight*width*width*7900;
	n2->weight+=halfWeight*width*width*7900;
//	std::cout<< "\nwas here 4" << std::flush;
}

/*Node* Bridge::findOrCreateNode(float x, float y) {
	for(unsigned int i=nnodes; i--;) {
		if(nodes[i].p.x==x && nodes[i].p.y==y) {
			return &(nodes[i]);
		}
	}
	return addNewNode(x, y);
}*/

/*void Bridge::findOrCreateNode1(Node** n, float x, float y) {
	*n=0;
	for(int i=nnodes; i--;) {
		if(nodes[i].b.x==x && nodes[i].b.y==y)
			*n=&(nodes[i]);
	}
	if(*n==0) {
		if(nnodes==nodesArrayLength) {
			Node* nn = new Node[nnodes+10];
			for(int i=nnodes; i--;)
				nn[i]=nodes[i];
			for(int i=nedges; i--;) {
				edges[i].from = &(nn[ edges[i].from-nodes ]);
				edges[i].to = &(nn[ edges[i].to-nodes ]);
				if(tfrom)
					tfrom = &( nn[tfrom-nodes ]);
				if(tto)
					tto = &( nn[tto-nodes ]);
			}
			delete [] nodes;
			nodes=nn;
			nodesArrayLength+=10;
		}
		nodes[nnodes].set(x,y);
		*n=&(nodes[nnodes++]);
	}
}*/

void Bridge::findOrCreateNode2(Node** n1, float x1, float y1, Node** n2, float x2, float y2) {
	*n1=0; *n2=0;
	for(int i=nnodes; i--;) {
		if(nodes[i].b.x==x1 && nodes[i].b.y==y1)
			*n1=&(nodes[i]);
		if(nodes[i].b.x==x2 && nodes[i].b.y==y2)
			*n2=&(nodes[i]);
	}
	if(*n1==0) {
		if(nnodes==nodesArrayLength) {
			Node* nn = new Node[nnodes+10];
			for(int i=nnodes; i--;)
				nn[i]=nodes[i];
			for(int i=nedges; i--;) {
				edges[i].from = &(nn[ edges[i].from-nodes ]);
				edges[i].to = &(nn[ edges[i].to-nodes ]);
			}
			if(*n2)
				*n2 = &(nn[ *n2-nodes ]);
			delete [] nodes;
			nodes=nn;
			nodesArrayLength+=10;
		}
		nodes[nnodes].set(x1,y1);
		*n1=&(nodes[nnodes++]);
	}
	if(*n2==0) {
		if(nnodes==nodesArrayLength) {
			Node* nn = new Node[nnodes+10];
			for(int i=nnodes; i--;)
				nn[i]=nodes[i];
			for(int i=nedges; i--;) {
				edges[i].from = &(nn[ edges[i].from-nodes ]);
				edges[i].to = &(nn[ edges[i].to-nodes ]);
			}
			*n1 = &(nn[ *n1-nodes ]);
			delete [] nodes;
			nodes=nn;
			nodesArrayLength+=10;
		}
		nodes[nnodes].set(x2,y2);
		*n2=&(nodes[nnodes++]);
	}
}

/*Node* Bridge::addNewNode(float x, float y) {
	if(nnodes==nodesArrayLength) {
		Node* nn = new Node[nnodes+10];
		for(int i=nnodes; i--;)
			nn[i]=nodes[i];

		for(int i=nedges; i--;) {
			edges[i].from = &(nn[ edges[i].from-nodes ]);
			edges[i].to = &(nn[ edges[i].to-nodes ]);
			where = &( nn[ where-nodes ]);
		}
		delete [] nodes;
		nodes=nn;
		nodesArrayLength+=10;
	}
	nodes[nnodes].set(x,y);
	return &(nodes[nnodes++]);
}*/

void Bridge::oneTimeStep(double time, Level* level) {
	for(int i=nedges; i--;) {
		if(edges[i].setForces() > 1.0f)
			removeEdge(i);
	}
	for(int i=nnodes; i--;) {
		if(!(level->contains(nodes+i)))
			nodes[i].move(time);
	}
	for(int i=nnodes; i--;) {
		if(!(level->contains(nodes+i)))
			nodes[i].update();
	}
}

void Bridge::removeEdge(unsigned int e) {
	Node* n1=edges[e].from;
	Node* n2=edges[e].to;
	float width=edges[e].width;
	float halfWeight=sqrt(pow(n1->p.x-n2->p.x,2)+pow(n1->p.y-n2->p.y,2))/2.0f;
	n1->weight-=halfWeight*width*width*7900;
	n2->weight-=halfWeight*width*width*7900;
	for(int i=e; i<nedges-1; ++i) {
		edges[i]=edges[i+1];
	}
	--nedges;
	if(n1->weight==0) 
		removeNode(n1); // todo remove node securely
	if(n2->weight==0) 
		removeNode(n2); // todo remove node securely
}

void Bridge::removeNode(Node* n) {
	for(int i=(n-nodes); i<nnodes-1; ++i) {
		nodes[i]=nodes[i+1];
	}
	for(int i=nedges; i--;) {
//		if((edges[i].from-nodes) > (n-nodes))
		if(edges[i].from > n)
			edges[i].from-=1;
//			edges[i].from = &(nodes[edges[i].from-nodes-1]);
//		if(edges[i].to-nodes > (n-nodes))
		if(edges[i].to > n)
			edges[i].to-=1;// &(nodes[edges[i].to-nodes-1]);
	}
	--nnodes;
}

void Bridge::liveRemove(int fx, int fy, int tx, int ty) {
	for(int i=nedges; i--;) {
		if((edges[i].from->b.x==fx && edges[i].from->b.y==fy && edges[i].to->b.x==tx && edges[i].to->b.y==ty) || (edges[i].from->b.x==tx && edges[i].from->b.y==ty && edges[i].to->b.x==fx && edges[i].to->b.y==fy))
			removeEdge(i);
	}
}

void Bridge::click(unsigned int cx, unsigned int cy) {
		float x = (float)cx - 24.0f;
		float y = (float)cy - 13.0f;
	if(!tfrom) {
		Node* near = 0;
		float distance = 100000000;
		float tmpDist;
		for(int i=nnodes; i--;) {
			tmpDist = sqrt(pow(nodes[i].p.x-x,2)+pow(nodes[i].p.y-y,2));
		//	std::cout << "\ntmpDist " << tmpDist <<" i " << i << " nodes[i].p.y " << nodes[i].p.y << " p.x " << (nodes[i].p.x) << " x " << x ;
			if(tmpDist<distance) {
				distance=tmpDist;
				near=nodes+i;
			}
		}
//		std::cout<<"bla"<<std::endl;
		tfrom=near;
//		std::cout<<"\n  "<<where<<"  "<<near;
//		std::cout<<"\nbla2"<<std::endl;
	}
	else {
		if(x != tfrom->p.x || y != tfrom->p.y) {
			addEdge(0.2f,tfrom->p.x,tfrom->p.y,x,y);
			std::cout<<"\nadded new edge\n";
			for(int i=10; i-->nedges-10;)
//				std::cout<<"\t";
			for(int i=nedges; i--;) {
//				std::cout<<nodes[i].p.x<<" "<<nodes[i].p.y<<"\t";
//				std::cout<<edges[i].from->b.x<<" "<<edges[i].to->b.x<<"\t";
			}
		}
		delete tfrom;
		tfrom=0;
	}
}

void Bridge::click3(unsigned int cx, unsigned int cy, float thickness) {
	float x = (float)cx - 24.0f;
	float y = (float)cy - 13.0f;
	if(!tfrom) {
		Node* near = new Node();
		near->set(x,y);
		tfrom=near;
	} else {
		if(x != tfrom->p.x || y != tfrom->p.y) {
			addEdge(thickness,tfrom->p.x,tfrom->p.y,tto->p.x,tto->p.y);
			stopClicking();
		}
	}
}

void Bridge::stopClicking() {
	if(tfrom) {
		delete tfrom;
		tfrom = 0;
		if(tto) {
			delete tto;
			tto = 0;
		}
		if(tEdge) {
			delete tEdge;
			tEdge = 0;
		}
	}
}

