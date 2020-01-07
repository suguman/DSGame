SLURMDIR = "../SLURMFILES/"

benchmarkdir = "../finalbenchmarks/"

outputdir = "../output/"

srcfile = "../src/./game"

df = 2

v = 3

## TIMEOUT FORMAT :: HH:MM:SS
timeout = "1:00:00"
## MEMOUT FORMAT :: NUM[G/M/B]
memout = "8G"

sep = "\n"

fname = open("benchmarksfilename.txt")

line = fname.readline().strip()

while(line!=""):

    filenametemp = line.split(":")[0].split("/")
    filename = "/".join([filenametemp[2], filenametemp[3]])
    print(filename)

    inputfilename = benchmarkdir + filename
    outputfilename = outputdir + filename

    commandline = " ".join(["srun", "time", srcfile, inputfilename, str(df), str(v)])

    cmd = "#!/bin/bash" + sep
    cmd += "#SBATCH --job-name=" + filename + sep
    cmd += "#SBATCH --account=commons" + sep
    cmd += "#SBATCH --partition=commons" + sep
    cmd += "#SBATCH --time=" + timeout  + sep
    cmd += "#SBATCH --mem-per-cpu=" + memout + sep
    cmd += "#SBATCH --mail-user=" + sep
    cmd += "#SBATCH --mail-type=ALL" + sep
    cmd += "#SBATCH --ntasks=1" + sep
    cmd += "#SBATCH --nodes=1" + sep
    cmd += "#SBATCH --ntasks-per-node=1 --exclusive" + sep
    cmd += "#SBATCH --cpus-per-task=1" + sep 
    cmd += sep + sep
    cmd += "echo \"" + inputfilename +"\"" + sep
    cmd += sep + sep
    cmd += commandline + sep
    cmd += "echo \"COMPLETED-DONE\"" + sep
    
    slurmfilename = SLURMDIR + filenametemp[2]+"-"+filenametemp[3] + ".slurm"
    f = open(slurmfilename, "w+")
    f.close()
    f = open(slurmfilename, "w+")
    f.write(cmd)
    f.close()
        
    line = fname.readline().strip()

"""
bnamefile.close()

for classname in benchmarkclass:
    for i in range(1,benchmarknum+ 1):
        #rootname = classname + str(i)
        if classname == "counter":
            inputfilename = benchmarkdir + classname + "/counter_" + str(i) + ".ltlf"
            inputpartfilename = benchmarkdir + classname + "/counter_" + str(i) + ".part"
        elif classname == "dcounter":
            inputfilename = benchmarkdir + classname + "/counters_" + str(i) + ".ltlf"
            inputpartfilename = benchmarkdir + classname + "/counters_" + str(i) + ".part"
        else:
            inputfilename = benchmarkdir + classname + "/" + str(i) + ".ltlf"
            inputpartfilename = benchmarkdir + classname + "/" + str(i) + ".part"

        # MONA
        #commandoptions = "srun time ../../ltlf2dfa.sh " + inputfilename 

        # EXPLICIT + Symbolic
        #commandoptions = " ".join(["srun", "time", "../../tool/./lisa",  "-ltlf", inputfilename])

        # Symbolic
        commandoptions = " ".join(["srun", "time", "../../tool/./lisa",  "-ltlf", inputfilename, "-nia 0", "-npa 0"])

        # EXPLICIT
        #commandoptions = " ".join(["srun", "time", "../../tool/./lisa",  "-ltlf", inputfilename, "-exp"])

        # LAST gets minimized
        #commandoptions = " ".join(["srun", "time", "../../tool/./lisa",  "-ltlf", inputfilename, "-last"])

        # SYN -- synthesize the strategy
        #commandoptions = " ".join(["srun", "time", "../../tool/./lisa",  "-ltlf", inputfilename, "-part", inputpartfilename, "-syn", "-env"])


        # SYFT 
        #commandoptions = " ".join(["srun", "time", "../../Syft/build/bin/Syft",   inputfilename, inputpartfilename, "1"])

        cmd = "#!/bin/bash" + sep
        cmd += "#SBATCH --job-name=" + classname+"-"+str(i) + sep
        cmd += "#SBATCH --account=commons" + sep
        cmd += "#SBATCH --partition=commons" + sep
        cmd += "#SBATCH --time=" + timeout  + sep
        cmd += "#SBATCH --mem-per-cpu=" + memout + sep
        cmd += "#SBATCH --mail-user=" + sep
        cmd += "#SBATCH --mail-type=ALL" + sep
        cmd += "#SBATCH --ntasks=1" + sep
        cmd += "#SBATCH --nodes=1" + sep
        cmd += "#SBATCH --ntasks-per-node=1 --exclusive" + sep
        cmd += "#SBATCH --cpus-per-task=1" + sep 
        cmd += sep + sep
        cmd += "echo \"" + inputfilename +"\"" + sep
        cmd += sep + sep
        cmd += commandoptions + sep
        cmd += "echo \"COMPLETED-DONE\"" + sep

        slurmfilename = SLURMDIR + classname+ "-"+str(i) + ".slurm"
        f = open(slurmfilename, "w+")
        f.close()
        f = open(slurmfilename, "w+")
        f.write(cmd)
        f.close()

