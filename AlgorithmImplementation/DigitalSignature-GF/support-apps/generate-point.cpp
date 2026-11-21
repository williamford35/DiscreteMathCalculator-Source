int main()
{
    ifstream common("common.ecs");    /* construct file I/O streams */
    ifstream private_key("private.ecs");
    ifstream message;
    ofstream signature;
    char ifname[50],ofname[50];
    ECn G;
    Big a,b,p,q,x,y,h,r,s,d,k;
    long seed;
    int bits;
    miracl *mip=&precision;

/* randomise */
    cout << "Enter 9 digit random number seed  = ";
    cin >> seed;
    irand(seed);

/* get common data */
    common >> bits;
    mip->IOBASE=16;
    common >> p >> a >> b >> q >> x >> y;
    mip->IOBASE=10;

/* calculate r - this can be done off-line,
   and hence amortized to almost nothing    */
    ecurve(a,b,p,MR_PROJECTIVE);
    G=ECn(x,y);
    k=rand(q);
    G*=k;            /* see ebrick.cpp for technique to speed this up */
    G.get(r);
    r%=q;

/* get private key of recipient */
    private_key >> d;

/* get message */
    cout << "file to be signed = " ;
    cin >> ifname;
    strcpy(ofname,ifname);
    strip(ofname);
    strcat(ofname,".ecs");
    message.open(ifname,ios::binary|ios::in); 
    if (!message)
    {
        cout << "Unable to open file " << ifname << "\n";
        return 0;
    }
    h=Hash(message);

/* calculate s */
    k=inverse(k,q);
    s=((h+d*r)*k)%q;
    signature.open(ofname);
    signature << r << endl;
    signature << s << endl;
    return 0;
}
