class License
{
	private:
		int nlicenses;
	public:

		int getlicense(void){
			while(nlicenses == 0);
			return(0);
		}		

		int returnlicense(void){
			nlicenses++;
			return(0);
		}	

		int initlicense(void){
			//do something
		}

		addtolicense(int n){
			nlicenses += n;
		}

		removelicense(int n){
			nlicenses -= n;
		}

		logmsg(const char * msg){

		}
}
