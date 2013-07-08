#include "dataio.h"

extern unsigned int total_populations;
extern struct POPULATION *population;
extern struct SYNAPSES **synapses;
extern struct DYNAMICS *Conductances_Models;
extern unsigned int total_syn_models;
extern struct DYNAMICS *Plasticity_Models;
extern unsigned int total_plas_models;
extern int **conn_values;
extern int **fdms_values;
extern double **delays_values;
extern double ***dist_values;
extern int ***RANDOM_DIST;
extern bool DivergentConnectivity;
extern bool Homogeneity;
extern bool Auto_connections;
extern bool Multiplicity;

int Save_Netbuilder_File(char *network_file){
  FILE *out_file;
  unsigned int pop, i, j;
  long fdm, par;

  out_file=fopen(network_file,"w+");
  if (out_file == NULL) {
	printf("%s","You need to have a valid data file.");
	exit(-1);
	Free_Memory();
  }

  /// Put a Header in the file with generation info
  fprintf(out_file, "# NetBuilder [ver. 1.0]");
  fprintf(out_file, "\n# START\n");

  fprintf(out_file,"\nPOPULATIONS %d", total_populations);
  fprintf(out_file,"\n");

  ///POPULATIONS
  for(pop=0; pop<total_populations; pop++) {

	fprintf(out_file,"\nPOPULATION %d :: %s", pop, population[pop].label);
	fprintf(out_file,"\n\tFDOMAINS %d", population[pop].total_fdomains);
	for(fdm=0; fdm<population[pop].total_fdomains; fdm++) {
	  fprintf(out_file,"\n\t\tFDOMAIN %ld :: %s", population[pop].fdomains[fdm].fdm_ID, population[pop].fdomains[fdm].label);
	  if (fdm !=0){
		fprintf(out_file," <- %ld :: %s", population[pop].fdomains[fdm].parent_ID,population[pop].fdomains[fdm].parent_label);
	  }

	  fprintf(out_file,"\t(%f,%f,%f,%f)",population[pop].fdomains[fdm].x,population[pop].fdomains[fdm].y, population[pop].fdomains[fdm].z, population[pop].fdomains[fdm].r);
	  fprintf(out_file,"\t%.3f", population[pop].fdomains[fdm].Ra);
	  fprintf(out_file,"\n\t\t\tDYNAMIC %s id=%d", population[pop].fdomains[fdm].dynamics.model_label,
			  population[pop].fdomains[fdm].dynamics.model_ID);
	  fprintf(out_file,"\n\t\t\t\tPARAMETERS %d", population[pop].fdomains[fdm].dynamics.model_total_parameters);
	  for(par=0; par<population[pop].fdomains[fdm].dynamics.model_total_parameters; par++) {
		fprintf(out_file,"\n\t\t\t\t\t%s %lf", population[pop].fdomains[fdm].dynamics.parameter_label[par],
				population[pop].fdomains[fdm].dynamics.parameter[par]);
	  }
	  fprintf(out_file,"\n\t\t\t\tSTATE_VARS %d", population[pop].fdomains[fdm].dynamics.n_state_vars);
	  for(int var=0; var<population[pop].fdomains[fdm].dynamics.n_state_vars; var++) {
		fprintf(out_file,"\t%s %lf", population[pop].fdomains[fdm].dynamics.state_vars_label[var], population[pop].fdomains[fdm].dynamics.state_vars_val[var]);
	  }
	}
	fprintf(out_file,"\n");
	fprintf(out_file,"\n\tTOPOLOGY");
	fprintf(out_file,"\n\t\tsize=%ld*%ld*%ld origin=(%f,%f,%f) dist=(%f,%f)",
			population[pop].topology.dim_x,
			population[pop].topology.dim_y,
			population[pop].topology.dim_z,
			population[pop].topology.X0,
			population[pop].topology.Y0,
			population[pop].topology.Z0,
			population[pop].topology.d_mean,
			population[pop].topology.d_std);

  }

  ///CONNECTIVITY
  fprintf(out_file,"\n");
  if (Auto_connections ==true){
	if (DivergentConnectivity==true && Homogeneity==true && Multiplicity==true)
	  fprintf(out_file,"\nCONNECTIVITY :: divergent - homogeneous - autoconnections - multiplicity");
	else if (DivergentConnectivity==false && Homogeneity==true && Multiplicity==true)
	  fprintf(out_file,"\nCONNECTIVITY :: convergent - homogeneous - autoconnections - multiplicity");
	else if (DivergentConnectivity==true && Homogeneity==false && Multiplicity==true)
	  fprintf(out_file,"\nCONNECTIVITY :: divergent - heterogeneous - autoconnections - multiplicity");
	else if (DivergentConnectivity==false && Homogeneity==false && Multiplicity==true)
	  fprintf(out_file,"\nCONNECTIVITY :: convergent - heterogeneous - autoconnections - multiplicity");
	else if (DivergentConnectivity==true && Homogeneity==true && Multiplicity==false)
	  fprintf(out_file,"\nCONNECTIVITY :: divergent - homogeneous - autoconnections - nomultiplicity");
	else if (DivergentConnectivity==false && Homogeneity==true && Multiplicity==false)
	  fprintf(out_file,"\nCONNECTIVITY :: convergent - homogeneous - autoconnections - nomultiplicity");
	else if (DivergentConnectivity==true && Homogeneity==false && Multiplicity==false)
	  fprintf(out_file,"\nCONNECTIVITY :: divergent - heterogeneous - autoconnections - nomultiplicity");
	else if (DivergentConnectivity==false && Homogeneity==false && Multiplicity==false)
	  fprintf(out_file,"\nCONNECTIVITY :: convergent - heterogeneous - autoconnections - nomultiplicity");
  }
  else{
	if (DivergentConnectivity==true && Homogeneity==true && Multiplicity==true)
	  fprintf(out_file,"\nCONNECTIVITY :: divergent - homogeneous - noautoconnections - multiplicity");
	else if (DivergentConnectivity==false && Homogeneity==true && Multiplicity==true)
	  fprintf(out_file,"\nCONNECTIVITY :: convergent - homogeneous - noautoconnections - multiplicity");
	else if (DivergentConnectivity==true && Homogeneity==false && Multiplicity==true)
	  fprintf(out_file,"\nCONNECTIVITY :: divergent - heterogeneous - noautoconnections - multiplicity");
	else if (DivergentConnectivity==false && Homogeneity==false && Multiplicity==true)
	  fprintf(out_file,"\nCONNECTIVITY :: convergent - heterogeneous - noautoconnections - multiplicity");
	else if (DivergentConnectivity==true && Homogeneity==true && Multiplicity==false)
	  fprintf(out_file,"\nCONNECTIVITY :: divergent - homogeneous - noautoconnections - nomultiplicity");
	else if (DivergentConnectivity==false && Homogeneity==true && Multiplicity==false)
	  fprintf(out_file,"\nCONNECTIVITY :: convergent - homogeneous - noautoconnections - nomultiplicity");
	else if (DivergentConnectivity==true && Homogeneity==false && Multiplicity==false)
	  fprintf(out_file,"\nCONNECTIVITY :: divergent - heterogeneous - noautoconnections - nomultiplicity");
	else if (DivergentConnectivity==false && Homogeneity==false && Multiplicity==false)
	  fprintf(out_file,"\nCONNECTIVITY :: convergent - heterogeneous - noautoconnections - nomultiplicity");
  }

  char x[3];
  char distrib[MAXLABELSIZE];

  for(i=0; i<total_populations; i++) {
	fprintf(out_file,"\n");
	for(j=0; j<total_populations; j++) {
	  for (int k = 0; k<3; k++){
		if ( RANDOM_DIST[i][j][k] == 0)
		  x[k]='N';
		else
		  x[k]='U';
	  }
	  sprintf(distrib,"(%c,%c,%c)",x[0],x[1],x[2]);
	  fprintf(out_file,"\t[%d %d (%f, %f, %f) %s %f]", conn_values[i][j], fdms_values[i][j], dist_values[i][j][0], dist_values[i][j][1], dist_values[i][j][2], distrib, delays_values[i][j]);
	}
  }

  ///SYNAPSES
  fprintf(out_file,"\n");
  fprintf(out_file,"\nSYNAPSES");

  for(i=0; i<total_populations; i++) {
	for(j=0; j<total_populations; j++) {

	  if( conn_values[i][j] != 0 ) {
		fprintf(out_file,"\n\tFIBERS :: %s->%s", population[i].label, population[j].label);
		fprintf(out_file,"\n\t\tCONDUCTANCES DYNAMICS %s id=%d", synapses[i][j].conductances_dynamics.model_label, synapses[i][j].conductances_dynamics.model_ID);
		fprintf(out_file,"\n\t\t\tPARAMETERS %d", synapses[i][j].conductances_dynamics.model_total_parameters);
		for(par=0; par<synapses[i][j].conductances_dynamics.model_total_parameters; par++) {
		  fprintf(out_file,"\n\t\t\t\t%s %lf", synapses[i][j].conductances_dynamics.parameter_label[par],
				  synapses[i][j].conductances_dynamics.parameter[par]);
		}
		fprintf(out_file,"\n\t\t\tSTATE_VARS %d", synapses[i][j].conductances_dynamics.n_state_vars);
		for(int var=0; var<synapses[i][j].conductances_dynamics.n_state_vars; var++) {
		    fprintf(out_file,"\t%s %lf", synapses[i][j].conductances_dynamics.state_vars_label[var],synapses[i][j].conductances_dynamics.state_vars_val[var]);
		}

		fprintf(out_file,"\n\t\tPLASTICITY DYNAMICS %s id=%d", synapses[i][j].plasticity_dynamics.model_label, synapses[i][j].plasticity_dynamics.model_ID);
		fprintf(out_file,"\n\t\t\tPARAMETERS %d", synapses[i][j].plasticity_dynamics.model_total_parameters);
		for(par=0; par<synapses[i][j].plasticity_dynamics.model_total_parameters; par++) {
		  fprintf(out_file,"\n\t\t\t\t%s %lf", synapses[i][j].plasticity_dynamics.parameter_label[par],
				  synapses[i][j].plasticity_dynamics.parameter[par]);
		}
		fprintf(out_file,"\n\t\t\tSTATE_VARS %d", synapses[i][j].plasticity_dynamics.n_state_vars);
		for(int var=0; var<synapses[i][j].plasticity_dynamics.n_state_vars; var++) {
		    fprintf(out_file,"\t%s %lf", synapses[i][j].plasticity_dynamics.state_vars_label[var], synapses[i][j].plasticity_dynamics.state_vars_val[var]);
		}

	  }
	}
  }
  fclose(out_file);
  return 0;
}

int Load_Netbuilder_File(char *network_file){
  FILE *in_file;
  int dummy, foo;
  char dummy1[MAXLABELSIZE];
	char *foocharp;

  in_file=fopen(network_file,"r");
  if (in_file == NULL) {
	printf("\n%s","You need to have a valid data file.");
	exit(-1);
	Free_Memory();
  }

  //Skip all comment lines.
  char temp_line[80]={};
  foocharp = fgets(temp_line, 80, in_file);
  while (strcmp(temp_line, "# START\n") != 0) {
	if (fgets(temp_line, 80, in_file) == NULL) {
	  printf("\n\tERROR: the keyword START was not found in _network_ file\n\n");
	  fclose(in_file);
	  return 1;
	}
  }

  ///POPULATIONS
  foo = fscanf(in_file,"\n");
  foo = fscanf(in_file,"\nPOPULATIONS %d", &total_populations);
  foo = fscanf(in_file,"\n");

  population=(POPULATION*) malloc(total_populations*sizeof(struct POPULATION));
  if (population == NULL) {
	printf("\n\tERROR: unable to allocate memory for list of POPULATIONS");
	printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
	exit(-1);
	Free_Memory();
  }

  for(unsigned int pop=0; pop<total_populations; pop++) {

	foo = fscanf(in_file,"\nPOPULATION %d :: %s", &dummy, population[pop].label);
	foo = fscanf(in_file,"\n\tFDOMAINS %d", &population[pop].total_fdomains);

	population[pop].fdomains=(FDOMAINS*) malloc(population[pop].total_fdomains*sizeof(struct FDOMAINS));
	if (population[pop].fdomains == NULL) {
	  printf("\n\tERROR: unable to allocate memory for list of FDOMAINS");
	  printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
		exit(-1);
		Free_Memory();
	}

	for(int fdm=0; fdm<population[pop].total_fdomains; fdm++) {
	  foo = fscanf(in_file,"\n\t\tFDOMAIN %ld :: %s", &population[pop].fdomains[fdm].fdm_ID, population[pop].fdomains[fdm].label);
	  if (fdm!=0)  foo = fscanf(in_file," <- %ld :: %s", &population[pop].fdomains[fdm].parent_ID, population[pop].fdomains[fdm].parent_label);
	  foo = fscanf(in_file,"\t(%lf,%lf,%lf,%lf)", &population[pop].fdomains[fdm].x, &population[pop].fdomains[fdm].y,  &population[pop].fdomains[fdm].z, &population[pop].fdomains[fdm].r);
	  foo = fscanf(in_file,"\t%lf", &population[pop].fdomains[fdm].Ra);
	  foo = fscanf(in_file,"\n\t\t\tDYNAMIC %s id=%d", population[pop].fdomains[fdm].dynamics.model_label,
			 &population[pop].fdomains[fdm].dynamics.model_ID);
	  foo = fscanf(in_file,"\n\t\t\t\tPARAMETERS %d", &population[pop].fdomains[fdm].dynamics.model_total_parameters);

	  population[pop].fdomains[fdm].dynamics.parameter=(double*) malloc(population[pop].fdomains[fdm].dynamics.model_total_parameters * sizeof(double));
	  population[pop].fdomains[fdm].dynamics.parameter_label=(char**) malloc( population[pop].fdomains[fdm].dynamics.model_total_parameters * sizeof(char*));
	  if (population[pop].fdomains[fdm].dynamics.parameter == NULL || population[pop].fdomains[fdm].dynamics.parameter_label == NULL ) {
		printf("\n\tERROR: unable to allocate memory for list of FDOMAINS");
		printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
		exit(-1);
		Free_Memory();
	  }

	  for(int par=0; par<population[pop].fdomains[fdm].dynamics.model_total_parameters; par++) {
		population[pop].fdomains[fdm].dynamics.parameter_label[par]=(char*) malloc( MAXLABELSIZE * sizeof(char));
		if (population[pop].fdomains[fdm].dynamics.parameter_label[par] == NULL) {
		  printf("\n\tERROR: unable to allocate memory for list of FDOMAINS");
		  printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
		  exit(-1);
		  Free_Memory();
		}
		foo = fscanf(in_file,"\n\t\t\t\t\t%s %lf", population[pop].fdomains[fdm].dynamics.parameter_label[par],
									  &population[pop].fdomains[fdm].dynamics.parameter[par]);
	  }

	  foo = fscanf(in_file,"\n\t\t\t\tSTATE_VARS %d", &population[pop].fdomains[fdm].dynamics.n_state_vars);
	  population[pop].fdomains[fdm].dynamics.state_vars_val=(double*) malloc( population[pop].fdomains[fdm].dynamics.n_state_vars * sizeof(double));
	  population[pop].fdomains[fdm].dynamics.state_vars_label=(char**) malloc( population[pop].fdomains[fdm].dynamics.n_state_vars * sizeof(char*));
	  if ( population[pop].fdomains[fdm].dynamics.state_vars_label == NULL || population[pop].fdomains[fdm].dynamics.state_vars_val == NULL ) {
		printf("\n\tERROR: unable to allocate memory for list of FDOMAINS");
			  printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
			  exit(-1);
			  Free_Memory();
	  }

	  for(int var=0; var<population[pop].fdomains[fdm].dynamics.n_state_vars; var++) {
		population[pop].fdomains[fdm].dynamics.state_vars_label[var]=(char*) malloc( MAXLABELSIZE * sizeof(char));
		if (population[pop].fdomains[fdm].dynamics.state_vars_label[var] == NULL) {
		  printf("\n\tERROR: unable to allocate memory for list of FDOMAINS");
		  printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
			  exit(-1);
			  Free_Memory();
		}
		foo = fscanf(in_file,"\t%s %lf" ,population[pop].fdomains[fdm].dynamics.state_vars_label[var], &population[pop].fdomains[fdm].dynamics.state_vars_val[var]);
	  }

	}
	foo = fscanf(in_file,"\n");
	foo = fscanf(in_file,"\n\tTOPOLOGY");
	foo = fscanf(in_file,"\n\t\tsize=%ld*%ld*%ld origin=(%lf,%lf,%lf) dist=(%lf,%lf)",
		   &population[pop].topology.dim_x,
		   &population[pop].topology.dim_y,
		   &population[pop].topology.dim_z,
		   &population[pop].topology.X0,
		   &population[pop].topology.Y0,
		   &population[pop].topology.Z0,
		   &population[pop].topology.d_mean,
		   &population[pop].topology.d_std);
  }

  ///CONNECTIVITY
  foo = fscanf(in_file,"\n");
  char diverg[MAXLABELSIZE];
  char homog[MAXLABELSIZE];
  char autoc[17];
  char mult[14];
  foo = fscanf(in_file,"\nCONNECTIVITY :: %s - %s - %s - %s",diverg, homog, autoc, mult);
  if (strcmp(diverg,"divergent")==0)
	DivergentConnectivity=true;
  else
	DivergentConnectivity=false;

  if (strcmp(homog,"homogeneous")==0)
	Homogeneity=true;
  else
	Homogeneity=false;

  if (strcmp(autoc,"autoconnections")==0)
	Auto_connections=true;
  else
	Auto_connections=false;

  if (strcmp(mult,"multiplicity")==0)
	Multiplicity=true;
  else
	Multiplicity=false;


  // connectivity matrix
  conn_values=(int**) malloc(total_populations * sizeof(int*));
  if (conn_values== NULL) {
	printf("\n\tERROR: unable to allocate memory for matrix Connectivity");
	printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
	exit(-1);
	Free_Memory();
  }
  for (unsigned int popi = 0; popi < total_populations; popi++) {
	conn_values[popi]=(int*) malloc(total_populations * sizeof(int));
	if (conn_values[popi]== NULL) {
	  printf("\n\tERROR: unable to allocate memory for matrix Connectivity");
	  printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
	  exit(-1);
	  Free_Memory();
	}
  }

  // target fdms matrix
  fdms_values=(int**) malloc(total_populations * sizeof(int*));
  if (fdms_values== NULL) {
	printf("\n\tERROR: unable to allocate memory for matrix FDMS");
	printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
	exit(-1);
	Free_Memory();
  }
  for (unsigned int popi = 0; popi < total_populations; popi++) {
	fdms_values[popi]=(int*) malloc(total_populations * sizeof(int));
	if (fdms_values[popi]== NULL) {
	  printf("\n\tERROR: unable to allocate memory for matrix FDMS");
	  printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
	  exit(-1);
	  Free_Memory();
	}
  }

  // distribution matrix
  dist_values=(double***) malloc(total_populations * sizeof(double**));
  RANDOM_DIST=(int***) malloc(total_populations * sizeof(int**));

  if (dist_values== NULL || RANDOM_DIST==NULL) {
	printf("\n\tERROR: unable to allocate memory for matrix Distributions");
	printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
	exit(-1);
	Free_Memory();
  }
  for (unsigned int popi = 0; popi < total_populations; popi++) {
	dist_values[popi]=(double**) malloc(total_populations * sizeof(double*));
	RANDOM_DIST[popi]=(int**) malloc(total_populations * sizeof(int*));
	if (dist_values[popi]== NULL || RANDOM_DIST[popi]==NULL ) {
	  printf("\n\tERROR: unable to allocate memory for matrix Distributions");
	  printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
	  exit(-1);
	  Free_Memory();
	}
  }

  // delays matrix
  delays_values=(double**) malloc(total_populations * sizeof(double*));
  if (delays_values== NULL) {
	printf("\n\tERROR: unable to allocate memory for matrix FDMS");
	printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
	exit(-1);
	Free_Memory();
  }
  for (unsigned int popi = 0; popi < total_populations; popi++) {
	delays_values[popi]=(double*) malloc(total_populations * sizeof(double));
	if (delays_values[popi]== NULL) {
	  printf("\n\tERROR: unable to allocate memory for matrix FDMS");
	  printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
	  exit(-1);
	  Free_Memory();
	}
  }

  for(unsigned int i=0; i<total_populations; i++) {
	foo = fscanf(in_file,"\n");
	for(unsigned int j=0; j<total_populations; j++) {
	  dist_values[i][j]= (double*) malloc(3 * sizeof(double));
	  RANDOM_DIST[i][j]=(int*) malloc(3 * sizeof(int));
	  if (dist_values[i][j]== NULL ||RANDOM_DIST[i][j]==NULL ) {
		printf("\n\tERROR: unable to allocate memory for matrix Distributions");
		printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
		exit(-1);
		Free_Memory();
	  }
	  char x[3];
	  foo = fscanf(in_file,"\t[%d %d (%lf, %lf, %lf) (%c,%c,%c) %lf]", &conn_values[i][j], &fdms_values[i][j], &dist_values[i][j][0],  &dist_values[i][j][1], &dist_values[i][j][2],&x[0],&x[1],&x[2],&delays_values[i][j]);

	  for (int k= 0;k<3;k++){
		if (x[k]=='U')
		  RANDOM_DIST[i][j][k]=1;
		else
		  RANDOM_DIST[i][j][k]=0;
	  }
	}
  }




  ///SYNAPSES
  foo = fscanf(in_file,"\n");
  foo = fscanf(in_file,"\nSYNAPSES");

  synapses=(struct SYNAPSES**) malloc(total_populations*sizeof(struct SYNAPSES*));
  if (synapses== NULL) {
		printf("\n\tERROR: unable to allocate memory for matrix Synapses");
		printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
		exit(-1);
		Free_Memory();
      }

      for (unsigned int pop = 0; pop < total_populations; pop++) {
		synapses[pop]=(struct SYNAPSES*) malloc(total_populations*sizeof(struct SYNAPSES));
		if (synapses[pop]== NULL) {
	      printf("\n\tERROR: unable to allocate memory for matrix Synapses");
	      printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
	      exit(-1);
	      Free_Memory();
		}
      }

      for(unsigned int i=0; i<total_populations; i++) {
	  for(unsigned int j=0; j<total_populations; j++) {
	      if( conn_values[i][j] != 0 ) {
		    foo = fscanf(in_file,"\n\tFIBERS :: %s->%s", dummy1, dummy1);
		    foo = fscanf(in_file,"\n\t\tCONDUCTANCES DYNAMICS %s id=%d", synapses[i][j].conductances_dynamics.model_label,
				&synapses[i][j].conductances_dynamics.model_ID);
		    foo = fscanf(in_file,"\n\t\t\tPARAMETERS %d", &synapses[i][j].conductances_dynamics.model_total_parameters);
		    synapses[i][j].conductances_dynamics.parameter=(double*) malloc( synapses[i][j].conductances_dynamics.model_total_parameters * sizeof(double));
		    synapses[i][j].conductances_dynamics.parameter_label=(char**) malloc( synapses[i][j].conductances_dynamics.model_total_parameters * sizeof(char*));
		    if (synapses[i][j].conductances_dynamics.parameter_label == NULL || synapses[i][j].conductances_dynamics.parameter==NULL) {
		      printf("\n\tERROR: unable to allocate memory for list of SYNAPSES");
		      printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
		      exit(-1);
		      Free_Memory();
		    }
		    for(int par=0; par<synapses[i][j].conductances_dynamics.model_total_parameters; par++) {
			synapses[i][j].conductances_dynamics.parameter_label[par]=(char*) malloc(MAXLABELSIZE * sizeof(char));
			if (synapses[i][j].conductances_dynamics.parameter_label[par] == NULL) {
				printf("\n\tERROR: unable to allocate memory for list of SYNAPSES");
				printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
				exit(-1);
				Free_Memory();
			}
			foo = fscanf(in_file,"\n\t\t\t\t%s %lf", synapses[i][j].conductances_dynamics.parameter_label[par],
					&synapses[i][j].conductances_dynamics.parameter[par]);
		    }
		    foo = fscanf(in_file,"\n\t\t\tSTATE_VARS %d", &synapses[i][j].conductances_dynamics.n_state_vars);
		    synapses[i][j].conductances_dynamics.state_vars_val=(double*) malloc( synapses[i][j].conductances_dynamics.n_state_vars * sizeof(double));
		    synapses[i][j].conductances_dynamics.state_vars_label=(char**) malloc( synapses[i][j].conductances_dynamics.n_state_vars * sizeof(char*));
		    if ( synapses[i][j].conductances_dynamics.state_vars_label == NULL || synapses[i][j].conductances_dynamics.state_vars_val == NULL) {
			  printf("\n\tERROR: unable to allocate memory for list of FDOMAINS");
				    printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
				    exit(-1);
				    Free_Memory();
		    }
		    for(int var=0; var<synapses[i][j].conductances_dynamics.n_state_vars; var++) {
			  synapses[i][j].conductances_dynamics.state_vars_label[var]=(char*) malloc( MAXLABELSIZE * sizeof(char));
			  if (synapses[i][j].conductances_dynamics.state_vars_label[var] == NULL) {
			    printf("\n\tERROR: unable to allocate memory for list of FDOMAINS");
			    printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
				    exit(-1);
				    Free_Memory();
			  }
			  foo = fscanf(in_file,"\t%s %lf" ,synapses[i][j].conductances_dynamics.state_vars_label[var], &synapses[i][j].conductances_dynamics.state_vars_val[var]);
		    }

		    foo = fscanf(in_file,"\n\t\tPLASTICITY DYNAMICS %s id=%d", synapses[i][j].plasticity_dynamics.model_label,
				&synapses[i][j].plasticity_dynamics.model_ID);
		    foo = fscanf(in_file,"\n\t\t\tPARAMETERS %d", &synapses[i][j].plasticity_dynamics.model_total_parameters);
		    synapses[i][j].plasticity_dynamics.parameter=(double*) malloc( synapses[i][j].plasticity_dynamics.model_total_parameters * sizeof(double));
		    synapses[i][j].plasticity_dynamics.parameter_label=(char**) malloc(synapses[i][j].plasticity_dynamics.model_total_parameters * sizeof(char*));
		    if (synapses[i][j].plasticity_dynamics.parameter_label == NULL || synapses[i][j].plasticity_dynamics.parameter == NULL) {
			printf("\n\tERROR: unable to allocate memory for list of SYNAPSES");
			printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
			exit(-1);
			Free_Memory();
		    }
		    for(int par=0; par<synapses[i][j].plasticity_dynamics.model_total_parameters; par++) {
			synapses[i][j].plasticity_dynamics.parameter_label[par]=(char*) malloc(MAXLABELSIZE * sizeof(char));
			if (synapses[i][j].plasticity_dynamics.parameter_label[par] == NULL) {
			    printf("\n\tERROR: unable to allocate memory for list of SYNAPSES");
			    printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
			    exit(-1);
			    Free_Memory();
			}
			foo = fscanf(in_file,"\n\t\t\t\t%s %lf", synapses[i][j].plasticity_dynamics.parameter_label[par],
				      &synapses[i][j].plasticity_dynamics.parameter[par]);
		    }
		    foo = fscanf(in_file,"\n\t\t\tSTATE_VARS %d", &synapses[i][j].plasticity_dynamics.n_state_vars);
		    synapses[i][j].plasticity_dynamics.state_vars_val=(double*) malloc( synapses[i][j].plasticity_dynamics.n_state_vars * sizeof(double));
		    synapses[i][j].plasticity_dynamics.state_vars_label=(char**) malloc( synapses[i][j].plasticity_dynamics.n_state_vars * sizeof(char*));
		    if ( synapses[i][j].plasticity_dynamics.state_vars_label == NULL || synapses[i][j].plasticity_dynamics.state_vars_val == NULL) {
			  printf("\n\tERROR: unable to allocate memory for list of FDOMAINS");
				    printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
				    exit(-1);
				    Free_Memory();
		    }
		    for(int var=0; var< synapses[i][j].plasticity_dynamics.n_state_vars; var++) {
			  synapses[i][j].plasticity_dynamics.state_vars_label[var]=(char*) malloc( MAXLABELSIZE * sizeof(char));
			  if (synapses[i][j].plasticity_dynamics.state_vars_label[var] == NULL) {
			    printf("\n\tERROR: unable to allocate memory for list of FDOMAINS");
			    printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
				    exit(-1);
				    Free_Memory();
			  }
			  foo = fscanf(in_file,"\t%s %lf" ,synapses[i][j].plasticity_dynamics.state_vars_label[var], &synapses[i][j].plasticity_dynamics.state_vars_val[var]);
		    }
	      }
	      else{
		    synapses[i][j].conductances_dynamics.model_ID=Conductances_Models[0].model_ID;
		    str_equal(Conductances_Models[0].model_label,synapses[i][j].conductances_dynamics.model_label);
		    synapses[i][j].conductances_dynamics.model_total_parameters=Conductances_Models[0].model_total_parameters;
		    synapses[i][j].conductances_dynamics.parameter=(double*) malloc(Conductances_Models[0].model_total_parameters*sizeof(double));
		    synapses[i][j].conductances_dynamics.parameter_label=(char**) malloc(Conductances_Models[0].model_total_parameters*sizeof(char*));
		    if (synapses[i][j].conductances_dynamics.parameter == NULL || synapses[i][j].conductances_dynamics.parameter_label==NULL)
		      {
			    printf("\n\tERROR: unable to allocate memory for list of FDOMAINS");
			    printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
			    exit(-1);
			    Free_Memory();
		      }
		    for (int p=0;p<Conductances_Models[0].model_total_parameters;p++)
		      {
			    synapses[i][j].conductances_dynamics.parameter_label[p]=(char*) malloc(MAXLABELSIZE * sizeof(char));
			    if ( synapses[i][j].conductances_dynamics.parameter_label[p] == NULL)
			      {
				    printf("\n\tERROR: unable to allocate memory for list of FDOMAINS");
				    printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
				    exit(-1);
				    Free_Memory();
			      }
			    str_equal(Conductances_Models[0].parameter_label[p], synapses[i][j].conductances_dynamics.parameter_label[p]);
			    synapses[i][j].conductances_dynamics.parameter[p]= Conductances_Models[0].parameter[p];
		      }
		      synapses[i][j].conductances_dynamics.n_state_vars=Conductances_Models[0].n_state_vars;
		      synapses[i][j].conductances_dynamics.state_vars_val=(double*) malloc( synapses[i][j].conductances_dynamics.n_state_vars * sizeof(double));
		      synapses[i][j].conductances_dynamics.state_vars_label=(char**) malloc( synapses[i][j].conductances_dynamics.n_state_vars * sizeof(char*));
		      if ( synapses[i][j].conductances_dynamics.state_vars_label == NULL || synapses[i][j].conductances_dynamics.state_vars_val == NULL ) {
			  printf("\n\tERROR: unable to allocate memory for list of FDOMAINS");
			  printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
			  exit(-1);
			  Free_Memory();
		      }
		      for(int var=0; var<synapses[i][j].conductances_dynamics.n_state_vars; var++) {
			  synapses[i][j].conductances_dynamics.state_vars_label[var]=(char*) malloc( MAXLABELSIZE * sizeof(char));
			  if (synapses[i][j].conductances_dynamics.state_vars_label[var] == NULL) {
			      printf("\n\tERROR: unable to allocate memory for list of FDOMAINS");
			      printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
			      exit(-1);
			      Free_Memory();
			  }
			  str_equal(Conductances_Models[0].state_vars_label[var], synapses[i][j].conductances_dynamics.state_vars_label[var]);
			  synapses[i][j].conductances_dynamics.state_vars_val[var]=Conductances_Models[0].state_vars_val[var];
			}


			///initialize synaptic plasticity

			synapses[i][j].plasticity_dynamics.model_ID=Plasticity_Models[0].model_ID;
			str_equal(Plasticity_Models[0].model_label,synapses[i][j].plasticity_dynamics.model_label);
			synapses[i][j].plasticity_dynamics.model_total_parameters=Plasticity_Models[0].model_total_parameters;
			synapses[i][j].plasticity_dynamics.parameter=(double*) malloc(Plasticity_Models[0].model_total_parameters*sizeof(double));
			synapses[i][j].plasticity_dynamics.parameter_label=(char**) malloc(Plasticity_Models[0].model_total_parameters*sizeof(char*));
			if (synapses[i][j].plasticity_dynamics.parameter == NULL || synapses[i][j].plasticity_dynamics.parameter_label==NULL){
			    printf("\n\tERROR: unable to allocate memory for list of FDOMAINS");
			    printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
			    exit(-1);
			    Free_Memory();
			}
			for (int p=0;p<Plasticity_Models[0].model_total_parameters;p++){
			    synapses[i][j].plasticity_dynamics.parameter_label[p]=(char*) malloc(MAXLABELSIZE * sizeof(char));
			    if ( synapses[i][j].plasticity_dynamics.parameter_label[p] == NULL) {
				printf("\n\tERROR: unable to allocate memory for list of FDOMAINS");
				printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
				exit(-1);
				Free_Memory();
			    }
			    str_equal(Plasticity_Models[0].parameter_label[p], synapses[i][j].plasticity_dynamics.parameter_label[p]);
			    synapses[i][j].plasticity_dynamics.parameter[p]= Plasticity_Models[0].parameter[p];
			}
			synapses[i][j].plasticity_dynamics.n_state_vars=Plasticity_Models[0].n_state_vars;
			synapses[i][j].plasticity_dynamics.state_vars_val=(double*) malloc( synapses[i][j].plasticity_dynamics.n_state_vars * sizeof(double));
			synapses[i][j].plasticity_dynamics.state_vars_label=(char**) malloc( synapses[i][j].plasticity_dynamics.n_state_vars * sizeof(char*));
			if ( synapses[i][j].plasticity_dynamics.state_vars_label == NULL ) {
			    printf("\n\tERROR: unable to allocate memory for list of FDOMAINS");
			    printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
			    exit(-1);
			    Free_Memory();
			}

			for(int var=0; var<synapses[i][j].plasticity_dynamics.n_state_vars; var++) {
			    synapses[i][j].plasticity_dynamics.state_vars_label[var]=(char*) malloc( MAXLABELSIZE * sizeof(char));
			    if (synapses[i][j].plasticity_dynamics.state_vars_label[var] == NULL) {
				printf("\n\tERROR: unable to allocate memory for list of FDOMAINS");
				printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
				exit(-1);
				Free_Memory();
			    }
			    str_equal(Plasticity_Models[0].state_vars_label[var],synapses[i][j].plasticity_dynamics.state_vars_label[var]);
			    synapses[i][j].plasticity_dynamics.state_vars_val[var]=Plasticity_Models[0].state_vars_val[var];
			}
		  }//close else
	  }//close j population target
      }//close i population source
      fclose(in_file);
      return 0;
}
