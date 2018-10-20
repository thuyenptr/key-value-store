import subprocess

def build_tree(t):
    print("\nCompile for t="+str(t)+"\n")
    p=subprocess.Popen(["make", "clean"], stdin=subprocess.PIPE, stdout=subprocess.PIPE)
    p=subprocess.Popen(["make"], stdin=subprocess.PIPE, stdout=subprocess.PIPE)
    out,err=p.communicate()
    #print(out)

    p=subprocess.Popen(["./Server", "-b"], stdin=subprocess.PIPE, stdout=subprocess.PIPE)
    out,err=p.communicate()
    print(out)

if __name__== "__main__":
    t_value = [x for x in range(3, 64)]
    for i in t_value:
        build_tree(i)
