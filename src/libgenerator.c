/**
 * \file libgenerator.c
 * \author Paulo Aguiar
 * \defgroup NeuronModels Neuron Models - models for fdomain dynamics
 * \defgroup SynapseModels Synapse Models - models for synaptic currents dynamics
 * \defgroup PlasticityModels Plasticity Models - models for synaptic plasticity dynamics
 */

#include <sys/types.h>
#include <sys/dir.h>
#include <sys/param.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


#define MAX_STRING_SIZE 50+1


/**
 * Holds information regarding a model
 */
struct HOLD_MODEL_DATA
{
  char *model_label;      ///< array holding all parameters labels
  int n_params;           ///< number of parameters
  char **param_label;     ///< array holding all parameters labels
  int n_statevars;        ///< number of state variables
  char **statevar_label;  ///< array holding all state variables labels
};

int total_neuron_models, total_synapse_models, total_plasticity_models;
char **neuron_models_label;
char **synapse_models_label;
char **plasticity_models_label;

// ---------------------------------------------------------------
//                            FUNCTIONS
// ---------------------------------------------------------------

void Create_Models_Library(char *homedir, char *modelstype)
{
	int i, total_models, noend_catch;
	char pathname[MAXPATHLEN];
  char temp_line[256];              //holds temporary data                  
  char *temp_ptr;                   //holds temporary data

	struct direct **files;
	FILE *out, *in;

	int File_Select();


	sprintf( pathname, "%s%s%s", homedir, "/NeuralSyns/dynamics/", modelstype);
	if( pathname == NULL ) {
		printf("Error getting %s directory path\n", modelstype);
		exit(0);
	}

	printf("\nSearching for models in directory: %s", pathname);

	total_models =	scandir( pathname, &files, File_Select, alphasort);

	/* If no files found, make a non-selectable menu item */
	if (total_models <= 0) {
		printf("\nNo .c models found in this directory!\n");
		exit(0);
	}
	
	printf("\n\tnumber of %s .c files = %d", modelstype, total_models);


	//create the models library
	sprintf( pathname, "%s%s%s%s", homedir, "/NeuralSyns/dynamics/", modelstype, ".lib");
	out = fopen( pathname, "w+");
  if ( out == NULL) {
    printf("\n\tERROR: unable to create %s.lib file in ~/NeuralSyns/dynamics\n", modelstype);
    exit(-1);
  }

	fprintf(out, "total models = %d\n", total_models);

	//go through all model .c files
	for( i=0; i<total_models; i++) {

		printf("\n\t- adding model defined in %s", files[i]->d_name );

		sprintf( pathname, "%s%s%s/%s", homedir, "/NeuralSyns/dynamics/", modelstype, files[i]->d_name);

		in = fopen( pathname, "r" );
		if( in == NULL ) {
			printf("\n\tERROR: unable to read %s file in ~/NeuralSyns/dynamics/%s\n", files[i]->d_name, modelstype);
			fclose(out);
			exit(-1);
		}

		fprintf(out, "\n");

		//copy contents from *in to *out
		temp_ptr = fgets(temp_line, 256, in);
		while( strcmp(temp_line, "MODEL DESCRIPTION:\n") != 0 ) {
			if( fgets(temp_line, 256, in) == NULL ) {
				printf("\n\tERROR: the keyword MODEL DESCRIPTION: was not found in the model file %s\n\n", files[i]->d_name);
				fclose(in);
				fclose(out);
				exit(-1);
			}
		}
		noend_catch = 0;
		temp_ptr = fgets(temp_line, 256, in);
		while( strcmp(temp_line, "END\n") != 0 ) {
			fputs(temp_line, out);
			noend_catch++;
			if( noend_catch > 1000 ) {
				printf("\n\tERROR: the keyword END was not found after MODEL DESCRIPTION in the file %s\n\n", files[i]->d_name);
				fclose(in);
				fclose(out);
				exit(-1);
			}
			temp_ptr = fgets(temp_line, 256, in);
		}
		
		//all done with this .c model; close it to start with new one
		fclose(in);

	}

	//all done with this models type
	fclose(out);

	return;
}


// ---------------------------------------------------------------


int File_Select(struct direct *entry)
{
	char *ptr;

	if( strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 ) {
		return 0;
	}
	
	//check for filename extensions; only want *.c files
	ptr = rindex(entry->d_name, '.');
	if( ptr != NULL && strcmp(ptr, ".c") == 0 ) {
		return 1;
	}
	else {
		return 0;
	}
}


//----------------------------------------------------------------


int main(int argc, char *argv[])
{
  int i, j;
  int read_error = 0;
  double trash;
	char *HOME_DIR;
  char chartrash[MAX_STRING_SIZE];
	char filedirectory[MAXPATHLEN];
  struct HOLD_MODEL_DATA model_data;
  FILE *neurons_lib_file = NULL;
  FILE *synapses_lib_file = NULL;
  FILE *plasticity_lib_file = NULL;
  FILE *out = NULL;



	//get home directory; NeuralSyns should be installed in the HOME directory!
	HOME_DIR = getenv("HOME");

	//CREATE ALL .lib FILES FROM THE AVAILABLE .c MODELS
	Create_Models_Library(HOME_DIR, "Neuron_Models");
	Create_Models_Library(HOME_DIR, "Synapse_Models");
	Create_Models_Library(HOME_DIR, "Plasticity_Models");



	sprintf( filedirectory,"%s%s", HOME_DIR, "/NeuralSyns/dynamics/Neuron_Models.lib");
  neurons_lib_file = fopen( filedirectory, "r");
  if ( neurons_lib_file == NULL) {
    printf("\n\tERROR: unable to find Neuron_Models.lib file in %s/NeuralSyns/dynamics\n", HOME_DIR);
    exit(-1);
  }

	sprintf( filedirectory,"%s%s", HOME_DIR, "/NeuralSyns/dynamics/Synapse_Models.lib");
  synapses_lib_file = fopen( filedirectory, "r");
  if ( synapses_lib_file == NULL) {
    printf("\n\tERROR: unable to find Synapse_Models.lib file in %s/NeuralSyns/dynamics\n", HOME_DIR);
    fclose(neurons_lib_file);
    exit(-1);
  }

	sprintf( filedirectory,"%s%s", HOME_DIR, "/NeuralSyns/dynamics/Plasticity_Models.lib");
  plasticity_lib_file = fopen( filedirectory, "r");
  if ( plasticity_lib_file == NULL) {
    printf("\n\tERROR: unable to find Plasticity_Models.lib file in %s/NeuralSyns/dynamics\n", HOME_DIR);
    fclose(neurons_lib_file);
    fclose(synapses_lib_file);
    exit(-1);
  }

  out = fopen("models.c", "w+");
  if ( out == NULL) {
    printf("\n\tERROR: unable to create models.c file in ~/NeuralSyns/src\n");
    fclose(neurons_lib_file);
    fclose(synapses_lib_file);
    fclose(plasticity_lib_file);
    exit(-1);
  }


  //CREATE HEADERS
  fprintf(out, "//************************************************************************\n");
  fprintf(out, "//*                                                                      *\n");
  fprintf(out, "//*                     M O D E L S   L I B R A R Y                      *\n");
  fprintf(out, "//*                                                                      *\n");
  fprintf(out, "//************************************************************************\n\n");

  fprintf(out, "//                            I M P O R T A N T                           \n");
  fprintf(out, "//                            -----------------                           \n");
  fprintf(out, "// THIS FILE IS GENERATED AUTOMATICALLY USING DATA IN THE MODELS LIBRARIES\n");
  fprintf(out, "//    Do not edit this file. This file is generated by libgenerator.c     \n");
  fprintf(out, "// -----------------------------------------------------------------------\n\n\n");

  fprintf(out, "#include <stdio.h>\n");
  fprintf(out, "#include <string.h>\n");
  fprintf(out, "#include <stdlib.h>\n\n");

  fprintf(out, "#include \"general.h\"\n");
  fprintf(out, "#include \"models.h\"\n\n\n");


  //TAKE CARE OF THE NEURON MODELS
  read_error = fscanf(neurons_lib_file,    "total models = %d", &total_neuron_models);
  read_error = fscanf(synapses_lib_file,   "total models = %d", &total_synapse_models);
  read_error = fscanf(plasticity_lib_file, "total models = %d", &total_plasticity_models);

  neuron_models_label = malloc( total_neuron_models * sizeof(char *) );
  synapse_models_label = malloc( total_synapse_models * sizeof(char *) );
  plasticity_models_label = malloc( total_plasticity_models * sizeof(char *) );


  fprintf(out, "//models\n");
  fprintf(out, "extern struct MODEL_DYNAMICS_NEURON *neuron_model;\n");
  fprintf(out, "extern struct MODEL_DYNAMICS_SYNAPSE *synapse_model;\n");
  fprintf(out, "extern struct MODEL_DYNAMICS_PLASTICITY *plasticity_model;\n\n\n\n");


  fprintf(out, "//------------------------------------------------------------------------\n");
  fprintf(out, "// Assign the appropriate function pointers to all models\n");
  fprintf(out, "void SetupModelsLibrary(void)\n");
  fprintf(out, "{\n");

  fprintf(out, "\tint n_params, n_statevars;");

  fprintf(out, "\n\n\t//allocate space for all models");
  fprintf(out, "\n\tneuron_model = malloc( N_NEURON_MODELS * sizeof(struct MODEL_DYNAMICS_NEURON) );");
  fprintf(out, "\n\tsynapse_model = malloc( N_SYNAPSE_MODELS * sizeof(struct MODEL_DYNAMICS_SYNAPSE) );");
  fprintf(out, "\n\tplasticity_model = malloc( N_PLASTICITY_MODELS * sizeof(struct MODEL_DYNAMICS_PLASTICITY) );");

  //-------------------------------------------------------------------------------------------------------------------------------------

  fprintf(out, "\n\n\n\t//TAKE CARE OF NEURON MODELS");
  fprintf(out, "\n\t//--------------------------");

  for(i=0; i<total_neuron_models; i++) {
    //read model label
    model_data.model_label = malloc( MAX_STRING_SIZE * sizeof(char) );
    read_error = fscanf(neurons_lib_file, "\n\n%s", model_data.model_label);
    neuron_models_label[i] = malloc( MAX_STRING_SIZE * sizeof(char) );
    strcpy( neuron_models_label[i], model_data.model_label);
    //read spike
    read_error = fscanf(neurons_lib_file, "\n\tSPIKE = %s", chartrash); //the SPIKE flag is only used by NetBuilder
    //read parameters
    read_error = fscanf(neurons_lib_file, "\n\tPARAM = %d:", &model_data.n_params);
    model_data.param_label = malloc( model_data.n_params * sizeof(char *) );
    for(j=0; j<model_data.n_params; j++) {    
      model_data.param_label[j] = malloc( MAX_STRING_SIZE * sizeof(char) );
      read_error = fscanf(neurons_lib_file, "\n\t\t%s = %lf", model_data.param_label[j], &trash);
    }
    //read state variables
    read_error = fscanf(neurons_lib_file, "\n\tSTATE = %d:", &model_data.n_statevars);
    model_data.statevar_label = malloc( model_data.n_statevars * sizeof(char *) );
    for(j=0; j<model_data.n_statevars; j++) {    
      model_data.statevar_label[j] = malloc( MAX_STRING_SIZE * sizeof(char));
      read_error = fscanf(neurons_lib_file, "\n\t\t%s = %lf", model_data.statevar_label[j], &trash);
    }
    //write file
    fprintf(out, "\n\n\t// model_%s", model_data.model_label);
    fprintf(out, "\n\tneuron_model[%d].model_label       = malloc( MAX_STRING_SIZE * sizeof( char ) );", i);  
	fprintf(out, "\n\tstrcpy( neuron_model[%d].model_label, \"%s\");", i, model_data.model_label);
    fprintf(out, "\n\tneuron_model[%d].Initialize        = ModelInitialize_%s;", i, model_data.model_label);
    fprintf(out, "\n\tneuron_model[%d].Advance           = ModelAdvance_%s;", i, model_data.model_label);

    fprintf(out, "\n\tn_params = %d;", model_data.n_params);
    fprintf(out, "\n\tneuron_model[%d].n_params          = %d;", i, model_data.n_params);
    fprintf(out, "\n\tneuron_model[%d].param_label       = malloc( n_params * sizeof( char * ) );", i);
    for(j=0; j<model_data.n_params; j++) {    
      fprintf(out, "\n\tneuron_model[%d].param_label[%d]    = malloc( MAX_STRING_SIZE * sizeof( char ) );", i, j);
      fprintf(out, "\n\tstrcpy( neuron_model[%d].param_label[%d], \"%s\");", i, j, model_data.param_label[j]);
    }

    fprintf(out, "\n\tn_statevars = %d;", model_data.n_statevars);
    fprintf(out, "\n\tneuron_model[%d].n_statevars       = %d;", i, model_data.n_statevars);
    if( model_data.n_statevars > 0 ) {
      fprintf(out, "\n\tneuron_model[%d].statevar_label    = malloc( n_statevars * sizeof( char * ) );", i);
      for(j=0; j<model_data.n_statevars; j++) {    
	fprintf(out, "\n\tneuron_model[%d].statevar_label[%d] = malloc( MAX_STRING_SIZE * sizeof( char ) );", i, j);
	fprintf(out, "\n\tstrcpy( neuron_model[%d].statevar_label[%d], \"%s\");", i, j, model_data.statevar_label[j]);
      }
    }
    else {
      fprintf(out, "\n\tneuron_model[%d].statevar_label    = NULL;", i);
    }

    free(model_data.param_label);
    free(model_data.statevar_label);
  }

  //-------------------------------------------------------------------------------------------------------------------------------------

  fprintf(out, "\n\n\n\t//TAKE CARE OF SYNAPSE MODELS");
  fprintf(out, "\n\t//---------------------------");

  for(i=0; i<total_synapse_models; i++) {
    //read model label
    model_data.model_label = malloc( MAX_STRING_SIZE * sizeof(char) );
    read_error = fscanf(synapses_lib_file, "\n\n%s", model_data.model_label);
    synapse_models_label[i] = malloc( MAX_STRING_SIZE * sizeof(char) );
	strcpy( synapse_models_label[i], model_data.model_label);
    //read parameters
    read_error = fscanf(synapses_lib_file, "\n\tPARAM = %d:", &model_data.n_params);
    model_data.param_label = malloc( model_data.n_params * sizeof(char *) );
    for(j=0; j<model_data.n_params; j++) {    
      model_data.param_label[j] = malloc( MAX_STRING_SIZE * sizeof(char));
      read_error = fscanf(synapses_lib_file, "\n\t\t%s = %lf", model_data.param_label[j], &trash);
    }
    //read state variables
    read_error = fscanf(synapses_lib_file, "\n\tSTATE = %d:", &model_data.n_statevars);
    model_data.statevar_label = malloc( model_data.n_statevars * sizeof(char *) );
    for(j=0; j<model_data.n_statevars; j++) {    
      model_data.statevar_label[j] = malloc( MAX_STRING_SIZE * sizeof(char));
      read_error = fscanf(synapses_lib_file, "\n\t\t%s = %lf", model_data.statevar_label[j], &trash);
    }
    //write file
    fprintf(out, "\n\n\t// model_%s", model_data.model_label);
    fprintf(out, "\n\tsynapse_model[%d].model_label       = malloc( MAX_STRING_SIZE * sizeof( char ) );", i);  
	fprintf(out, "\n\tstrcpy( synapse_model[%d].model_label, \"%s\");", i, model_data.model_label);
    fprintf(out, "\n\tsynapse_model[%d].Initialize        = ModelInitialize_%s;", i, model_data.model_label);
	fprintf(out, "\n\tsynapse_model[%d].Activate          = ModelActivate_%s;",   i, model_data.model_label);
    fprintf(out, "\n\tsynapse_model[%d].Advance           = ModelAdvance_%s;",    i, model_data.model_label);

    fprintf(out, "\n\tn_params = %d;", model_data.n_params);
    fprintf(out, "\n\tsynapse_model[%d].n_params          = %d;", i, model_data.n_params);
    fprintf(out, "\n\tsynapse_model[%d].param_label       = malloc( n_params * sizeof( char * ) );", i);
    for(j=0; j<model_data.n_params; j++) {    
      fprintf(out, "\n\tsynapse_model[%d].param_label[%d]    = malloc( MAX_STRING_SIZE * sizeof( char ) );", i, j);
      fprintf(out, "\n\tstrcpy( synapse_model[%d].param_label[%d], \"%s\");", i, j, model_data.param_label[j]);
    }

    fprintf(out, "\n\tn_statevars = %d;", model_data.n_statevars);
    fprintf(out, "\n\tsynapse_model[%d].n_statevars       = %d;", i, model_data.n_statevars);
    if( model_data.n_statevars > 0 ) {
      fprintf(out, "\n\tsynapse_model[%d].statevar_label    = malloc( n_statevars * sizeof( char * ) );", i);
      for(j=0; j<model_data.n_statevars; j++) {    
	fprintf(out, "\n\tsynapse_model[%d].statevar_label[%d] = malloc( MAX_STRING_SIZE * sizeof( char ) );", i, j);
	fprintf(out, "\n\tstrcpy( synapse_model[%d].statevar_label[%d], \"%s\");", i, j, model_data.statevar_label[j]);
      }
    }
    else {
      fprintf(out, "\n\tsynapse_model[%d].statevar_label    = NULL;", i);
    }
    
    free(model_data.param_label);
    free(model_data.statevar_label);
  }

  //-------------------------------------------------------------------------------------------------------------------------------------

  fprintf(out, "\n\n\n\t//TAKE CARE OF PLASTICITY MODELS");
  fprintf(out, "\n\t//------------------------------");

  for(i=0; i<total_plasticity_models; i++) {
    //read model label
    model_data.model_label = malloc( MAX_STRING_SIZE * sizeof(char) );
    read_error = fscanf(plasticity_lib_file, "\n\n%s", model_data.model_label);
    plasticity_models_label[i] = malloc( MAX_STRING_SIZE * sizeof(char) );
	strcpy( plasticity_models_label[i], model_data.model_label);
    //read parameters
    read_error = fscanf(plasticity_lib_file, "\n\tPARAM = %d:", &model_data.n_params);
    model_data.param_label = malloc( model_data.n_params * sizeof(char *) );
    for(j=0; j<model_data.n_params; j++) {    
      model_data.param_label[j] = malloc( MAX_STRING_SIZE * sizeof(char));
      read_error = fscanf(plasticity_lib_file, "\n\t\t%s = %lf", model_data.param_label[j], &trash);
    }
    //read state variables
    read_error = fscanf(plasticity_lib_file, "\n\tSTATE = %d:", &model_data.n_statevars);
    model_data.statevar_label = malloc( model_data.n_statevars * sizeof(char *) );
    for(j=0; j<model_data.n_statevars; j++) {    
      model_data.statevar_label[j] = malloc( MAX_STRING_SIZE * sizeof(char));
      read_error = fscanf(plasticity_lib_file, "\n\t\t%s = %lf", model_data.statevar_label[j], &trash);
    }
    //write file
    fprintf(out, "\n\n\t// model_%s", model_data.model_label);
    fprintf(out, "\n\tplasticity_model[%d].model_label       = malloc( MAX_STRING_SIZE * sizeof( char ) );", i);  
		fprintf(out, "\n\tstrcpy( plasticity_model[%d].model_label, \"%s\");", i, model_data.model_label);
    fprintf(out, "\n\tplasticity_model[%d].Initialize        = ModelInitialize_%s;", i, model_data.model_label);
    fprintf(out, "\n\tplasticity_model[%d].Activate          = ModelActivate_%s;",   i, model_data.model_label);

    fprintf(out, "\n\tn_params = %d;", model_data.n_params);
    fprintf(out, "\n\tplasticity_model[%d].n_params          = %d;", i, model_data.n_params);
    fprintf(out, "\n\tplasticity_model[%d].param_label       = malloc( n_params * sizeof( char * ) );", i);
    for(j=0; j<model_data.n_params; j++) {    
      fprintf(out, "\n\tplasticity_model[%d].param_label[%d]    = malloc( MAX_STRING_SIZE * sizeof( char ) );", i, j);
      fprintf(out, "\n\tstrcpy( plasticity_model[%d].param_label[%d], \"%s\");", i, j, model_data.param_label[j]);
    }

    fprintf(out, "\n\tn_statevars = %d;", model_data.n_statevars);
    fprintf(out, "\n\tplasticity_model[%d].n_statevars       = %d;", i, model_data.n_statevars);
    if( model_data.n_statevars > 0 ) {
      fprintf(out, "\n\tplasticity_model[%d].statevar_label    = malloc( n_statevars * sizeof( char * ) );", i);
      for(j=0; j<model_data.n_statevars; j++) {    
	fprintf(out, "\n\tplasticity_model[%d].statevar_label[%d]  = malloc( MAX_STRING_SIZE * sizeof( char ) );", i, j);
	fprintf(out, "\n\tstrcpy( plasticity_model[%d].statevar_label[%d], \"%s\");", i, j, model_data.statevar_label[j]);
      }
    }
    else {
      fprintf(out, "\n\tplasticity_model[%d].statevar_label    = NULL;", i);
    }
    

    free(model_data.param_label);
    free(model_data.statevar_label);
  }

  fprintf(out, "\n\n\n\treturn;");
  fprintf(out, "\n}\n");

  //close streams which are not longer needed
  fclose(neurons_lib_file);
  fclose(synapses_lib_file);
  fclose(plasticity_lib_file);



  //Now create the NEURON MODEL translation function
  fprintf(out, "\n\n\n//------------------------------------------------------------------------");
  fprintf(out, "\n// Get appropriate neuron model structure from the neuron model string");
  fprintf(out, "\nstruct MODEL_DYNAMICS_NEURON* GetNeuronModelFromString( char *modelname )");
  fprintf(out, "\n{");
  for(i=0; i<total_neuron_models; i++) {
    fprintf(out, "\n\tif( strcmp(\"%s\", modelname) == 0 ) {", neuron_models_label[i]);
    fprintf(out, "\n\t\treturn &neuron_model[%d];", i);
    fprintf(out, "\n\t}\n");
  }
  fprintf(out, "\n\t// it's not good at all if you arrive here empty handed...");
  fprintf(out, "\n\tprintf(\"\\n\\nERROR - It was not possible to associate a neuron (fdomain) modelname with the appropriate set of function pointers\\nThe error occurred with the modelname = %%s\\n\\nIf you added new models you need to run again NeuralSyns instalation script. Check also if the libraries in the 'dynamics' folder have errors\\n\", modelname);");
  fprintf(out, "\n\treturn NULL;");
  fprintf(out, "\n}\n");


  //Now create the SYNAPSE MODEL translation function
  fprintf(out, "\n\n\n//------------------------------------------------------------------------");
  fprintf(out, "\n// Get appropriate synapse model structure from the synapse model string");
  fprintf(out, "\nstruct MODEL_DYNAMICS_SYNAPSE* GetSynapseModelFromString( char *modelname )");
  fprintf(out, "\n{");
  for(i=0; i<total_synapse_models; i++) {
    fprintf(out, "\n\tif( strcmp(\"%s\", modelname) == 0 ) {", synapse_models_label[i]);
    fprintf(out, "\n\t\treturn &synapse_model[%d];", i);
    fprintf(out, "\n\t}\n");
  }
  fprintf(out, "\n\t// it's not good at all if you arrive here empty handed...");
  fprintf(out, "\n\tprintf(\"\\n\\nERROR - It was not possible to associate a synapse modelname with the appropriate set of function pointers\\nThe error occurred with the modelname = %%s\\n\\nIf you added new models you need to run again NeuralSyns instalation script. Check also if the libraries in the 'dynamics' folder have errors\\n\", modelname);");
  fprintf(out, "\n\treturn NULL;");
  fprintf(out, "\n}\n");

  //Now create the PLASTICITY MODEL translation function
  fprintf(out, "\n\n\n//------------------------------------------------------------------------");
  fprintf(out, "\n// Get appropriate plasticty model structure from the plasticity model string");
  fprintf(out, "\nstruct MODEL_DYNAMICS_PLASTICITY* GetPlasticityModelFromString( char *modelname )");
  fprintf(out, "\n{");
  for(i=0; i<total_plasticity_models; i++) {
    fprintf(out, "\n\tif( strcmp(\"%s\", modelname) == 0 ) {", plasticity_models_label[i]);
    fprintf(out, "\n\t\treturn &plasticity_model[%d];", i);
    fprintf(out, "\n\t}\n");
  }  
  fprintf(out, "\n\t// it's not good at all if you arrive here empty handed...");
  fprintf(out, "\n\tprintf(\"\\n\\nERROR - It was not possible to associate a plasticity modelname with the appropriate set of function pointers\\nThe error occurred with the modelname = %%s\\n\\nIf you added new models you need to run again NeuralSyns instalation script. Check also if the libraries in the 'dynamics' folder have errors\\n\", modelname);");
  fprintf(out, "\n\treturn NULL;");
  fprintf(out, "\n}\n");



  //all written
  fclose(out);

  printf("\n\nSUCCESS: The source file 'models.c' has been automatically generated from the model libraries in the 'dynamics' folder\n");
  fflush(stdout);


	//------------------------------------------------------------------------------------------------------------------------------------
  

  //NOW MOVE TO THE models.h FILE

  out = fopen("models.h", "w+");
  if ( out == NULL) {
    printf("\n\tERROR: unable to create models.h file in ~/NeuralSyns/src\n");
    exit(-1);
  }


  fprintf(out, "//************************************************************************");
  fprintf(out, "\n//*                                                                      *");
  fprintf(out, "\n//*                     M O D E L S   L I B R A R Y                      *");
  fprintf(out, "\n//*                                                                      *");
  fprintf(out, "\n//************************************************************************");


  fprintf(out, "//                            I M P O R T A N T                           \n");
  fprintf(out, "//                            -----------------                           \n");
  fprintf(out, "// THIS FILE IS GENERATED AUTOMATICALLY USING DATA IN THE MODELS LIBRARIES\n");
  fprintf(out, "//    Do not edit this file. This file is generated by libgenerator.c     \n");
  fprintf(out, "// -----------------------------------------------------------------------\n\n\n");
 

  fprintf(out, "\n\n\n#if defined(MODELS_H)");
  fprintf(out, "\n#else");
  fprintf(out, "\n#define MODELS_H");
  
  fprintf(out, "\n\n\n#include <gsl/gsl_errno.h>");
  fprintf(out, "\n#include <gsl/gsl_matrix.h>");
  fprintf(out, "\n#include <gsl/gsl_odeiv.h>");
  fprintf(out, "\n#include <gsl/gsl_sf_trig.h>");
  fprintf(out, "\n#include <gsl/gsl_math.h>");
  
  fprintf(out, "\n\n\n#define N_NEURON_MODELS %d", total_neuron_models);
  fprintf(out, "\n#define N_SYNAPSE_MODELS %d", total_synapse_models);
  fprintf(out, "\n#define N_PLASTICITY_MODELS %d", total_plasticity_models);
  
  fprintf(out, "\n\n\n\n//definitions and structures");

  fprintf(out, "\n\n/// holds all variables/parameters required to use the GNU Scientific Library ode solver");
  fprintf(out, "\nstruct GSL_ODESOLVER_DATA");
  fprintf(out, "\n{");
  fprintf(out, "\n\tdouble *y;");
  fprintf(out, "\n\tdouble *y_err;");
  fprintf(out, "\n\tdouble *dydt_in, *dydt_out;");
  fprintf(out, "\n\tconst gsl_odeiv_step_type *T;");
  fprintf(out, "\n\tgsl_odeiv_step *s;");
  fprintf(out, "\n\tgsl_odeiv_system sys;");
  fprintf(out, "\n};");
  
  fprintf(out, "\n\n/// holds all data regarding a particular model for fdomain dynamics, including pointers to relevant functions");
  fprintf(out, "\nstruct MODEL_DYNAMICS_NEURON");
  fprintf(out, "\n{");
  fprintf(out, "\n\tchar *model_label;                     ///< array holding all parameters labels");
  fprintf(out, "\n\tvoid (*Initialize)(long nrn, int fdm); ///< pointer to the model Initialize() function");
  fprintf(out, "\n\tdouble (*Advance)(long nrn, int fdm);  ///< pointer to the model Advance() function");  
  fprintf(out, "\n\tint n_params;                          ///< number of parameters defining the model dynamics");
  fprintf(out, "\n\tchar **param_label;                    ///< array holding all parameters labels");  
  fprintf(out, "\n\tint n_statevars;                       ///< number of state variables used in the model dynamics");
  fprintf(out, "\n\tchar **statevar_label;                 ///< array holding all state variables labels");
  fprintf(out, "\n};");

  fprintf(out, "\n\n/// holds all data regarding a particular model for synapse current dynamics, including pointers to relevant functions");
  fprintf(out, "\nstruct MODEL_DYNAMICS_SYNAPSE");
  fprintf(out, "\n{");
  fprintf(out, "\n\tchar *model_label;                                                         ///< array holding all parameters labels");
  fprintf(out, "\n\tvoid (*Initialize)(long syn, long pre_nrn, long post_nrn, int target_fdm); ///< pointer to the model Initialize() function");
  fprintf(out, "\n\tvoid (*Activate)(double activation_time, long syn, long pre_nrn, long post_nrn, int target_fdm);   ///< pointer to the model Activate() function");
  fprintf(out, "\n\tvoid (*Advance)(long syn, long pre_nrn, long post_nrn, int target_fdm);    ///< pointer to the model Advance() function");
  fprintf(out, "\n\tint n_params;                                                              ///< number of parameters defining the model dynamics");
  fprintf(out, "\n\tchar **param_label;                                                        ///< array holding all parameters labels");  
  fprintf(out, "\n\tint n_statevars;                                                           ///< number of state variables used in the model dynamics");
  fprintf(out, "\n\tchar **statevar_label;                                                     ///< array holding all state variables labels");
  fprintf(out, "\n};");
    
  fprintf(out, "\n\n/// holds all data regarding a particular model for synapse plasticity dynamics, including pointers to relevant functions");
  fprintf(out, "\nstruct MODEL_DYNAMICS_PLASTICITY");
  fprintf(out, "\n{");
  fprintf(out, "\n\tchar *model_label;                                                         ///< array holding all parameters labels");
  fprintf(out, "\n\tvoid (*Initialize)(long syn, long pre_nrn, long post_nrn, int target_fdm); ///< pointer to the model Initialize() function");
  fprintf(out, "\n\tvoid (*Activate)(double activation_time, long syn, long pre_nrn, long post_nrn, int target_fdm);   ///< pointer to the model Activate() function");
  fprintf(out, "\n\tint n_params;                                                              ///< number of parameters defining the model dynamics");
  fprintf(out, "\n\tchar **param_label;                                                        ///< array holding all parameters labels");  
  fprintf(out, "\n\tint n_statevars;                                                           ///< number of state variables used in the model dynamics");
  fprintf(out, "\n\tchar **statevar_label;                                                     ///< array holding all state variables labels");
  fprintf(out, "\n};");

  
  fprintf(out, "\n\n\n\n//Functions prototypes");
  fprintf(out, "\nvoid SetupModelsLibrary(void);\n");
  fprintf(out, "\nstruct MODEL_DYNAMICS_NEURON *GetNeuronModelFromString( char *modelname );");
  fprintf(out, "\nstruct MODEL_DYNAMICS_SYNAPSE *GetSynapseModelFromString( char *modelname );");
  fprintf(out, "\nstruct MODEL_DYNAMICS_PLASTICITY *GetPlasticityModelFromString( char *modelname );\n");
  

  //write function prototypes for all models
  for(i=0; i<total_neuron_models; i++) {
    fprintf(out, "\n\nvoid ModelInitialize_%s(long nrn, int fdm);", neuron_models_label[i]);
    fprintf(out, "\ndouble ModelAdvance_%s(long nrn, int fdm);", neuron_models_label[i]);
  }
  fprintf(out, "\n");
  for(i=0; i<total_synapse_models; i++) {
    fprintf(out, "\n\nvoid ModelInitialize_%s(long syn, long pre_nrn, long post_nrn, int target_fdm);", synapse_models_label[i]);
    fprintf(out, "\nvoid ModelActivate_%s(double activation_time, long syn, long pre_nrn, long post_nrn, int target_fdm);", synapse_models_label[i]);
    fprintf(out, "\nvoid ModelAdvance_%s(long syn, long pre_nrn, long post_nrn, int target_fdm);", synapse_models_label[i]);
  }
  fprintf(out, "\n");
  for(i=0; i<total_plasticity_models; i++) {
    fprintf(out, "\n\nvoid ModelInitialize_%s(long syn, long pre_nrn, long post_nrn, int target_fdm);", plasticity_models_label[i]);
    fprintf(out, "\nvoid ModelActivate_%s(double activation_time, long syn, long pre_nrn, long post_nrn, int target_fdm);", plasticity_models_label[i]);
  }


  fprintf(out, "\n\n\n\n#endif\n");

  //all written
  fclose(out);

  printf("\nSUCCESS: The source file 'models.h' has been automatically generated from the model libraries in the 'dynamics' folder\n\n");
  fflush(stdout);


  return 0;
}
