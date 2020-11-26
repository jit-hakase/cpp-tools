#include <stdio.h>
#include <pcap.h>
#include <netinet/tcp.h>

int main()
{
	pcap_file_header meta_hdr;
	pcap_pkthdr hdr;
	size_t sz;
	int idx;
	FILE *fp;
	
	fp = fopen("test.pcap", "rw");
	
	fread(&meta_hdr, sizeof(pcap_file_header), 1, fp);
	
	unsigned char buf[1480];
	
	while ((sz = fread(&hdr, sizeof(pcap_pkthdr), 1, fp)) > 0) {
		sz = fread(buf, 1, hdr.caplen, fp);
		if (sz != hdr.caplen) return -1;
		for (idx = 0; idx < sz; ++idx) {
			if (idx && idx%16==0) printf("\n");
			if (idx && idx%8==0) printf(" ");
			printf("%02hhx ", buf[idx]);
		}
		printf("\n\n");
	}
	
	fclose(fp);
	return 0;
}
