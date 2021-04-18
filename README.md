# Characterizing A Perceptron

The code trains and tests a simple classifier based on a single-level neural
net (or *perceptron*).  Perceptrons were one of the earliest forms of neural
network and are an important building block in model deep-learning networks.

This lab is quite long, but it doesn't require any coding.  There are quite a
lot of questions to answer, though, and the you'll need to run quite a few
experiments, which will take time (especially if you wait till the last minute
and the autograder is bogged down), so start early!

This lab will introduce you to the tools you will use throughout the rest of
class to understand the behavior of more and more complex machine learning
models.  It serves two purposes:

1. It is an advanced tutorial for running experiments on the
autograder infrastructure and interpreting the results.

2. It provides hands-on experience with the impact the compile and
system settings have on performance and efficiency.

Both are important for the course, but #1 is critical for your ability to do
the remaining labs.

This lab will be completed on your own.

Check gradescope for due date(s).

## Keeping the starter code in sync
Occasionally, there will be changes made to the base repository after the 
assignment is released. In those cases, you can use the following commands to 
pull the changes from upstream and merge into your code.
```
runlab --check-for-updates
runlab --merge-updates
```

## Grading

Your grade for this lab will be based on your completion of the data
collection steps described in this document and the completed worksheet.

| Part           | value |
|----------------|-------|
| Data collection| 25%   |
| Worksheet      | 75%   |

Your final score on the lab will the weighted average of your % score
on the gradescope assignment for each part.

## Skills to Learn and Practice

1. Use the autograder to measure the performance and performance counters for a test program.

2. Use the `gprof` profiler to understand where a program spends its time.

3. Analyze how the compiler converts your code to assembly.

4. Apply compiler optimizations to improve performance

5. Compare the performance of different versions of the test program.

6. Estimate the performance of the test program based on input characteristics.

7. Vary the clock speed at which the autograder runs.

8. Optimize the energy consumed by the test program.

9. Prepare legible graphs from gathered data and draw conclusions from them.

## Software You Will Need

1. A computer with Docker installed (either the cloud docker container
via ssh, or your own laptop).  See the intro lab for details.

2. The lab for the github classroom assignment for this lab.  Find the
link on the course home page: https://github.com/CSE141pp/Home/.

3. A PDF annotator/editor to fill out `worksheet.pdf`.  You'll
submit this via a *a separate assignment* in Gradescope.  We *will
not* look at the version in your repo.

## Tasks to Perform

### Fill Out the Worksheet

Be sure to fill out the entire worksheet.  The easiest way to fill it
out is with some sort of PDF annotation tool.  You have some
flexibility in how you fill it out as long as

1. The location of the answers remains the same

2. Your graphs and tables fit in the space provided.

You can, for instance, recreate the tables (and create the graphs) is
Excel and paste them in.

Hand-drawn graphs are not acceptable.

Hand-written/inked numbers are acceptable __if they are legible__.  We
won't spend much time deciphering your hand writing.

Be sure to follow the guidelines we discussed in class for creating
nice-looking, legible graphs.

**DO NOT** attach extra sheets.  Your work must fit in space provided.

Also, remember that you can only submit to the worksheet assignment
**once**, while you can resubmit the coding portion as often as you
would like.

### Run the Starter Code Locally and Verify the Output

Accept the starter code via the link above.

This will set you up with a copy of the starter repository.

Clone your repo locally.

```
git clone [link to your repo]
```

You'll find several files.  You will be editing these two:

1.  `code.cpp` -- The functions that actually do the work.
2.  `config.env` -- Configuration file the experiment you'll run.

### Test the Starter Code Locally

As you did in Lab 0, navigate to the clone of your repo while inside
the course development environment docker image.

Do `runlab` to build and run the code.

### Section. Exploring The Lab

Take a look at `Makefile` in the root of the repo.  This is the make file used
when your repo is run on the reference processor.

This lab will run two experiments: One to measure instruction mix
(`INST_MIX_CMD_LINE_ARGS`) and another to measure performance
(`PE_CMD_LINE_ARGS`).

```
INST_MIX_CMD_LINE_ARGS=--stat-set inst_mix.cfg $(CMD_LINE_ARGS)
```

Let's break that down:

* `--stat-set inst_mix.cfg`: This tells the autograder to measure
   instruction mix information.

*  `$(CMD_LINE_ARGS)`: This will expand to what you set in `config.env`.

The instruction mix stats it collects will appear in `inst_mix.csv`
after your code runs (you'll be able to find it via the zip file URL
the autograde will give you).

Performance data is collected with `--stat-set PE.cfg`.  It ends up in
`pe.csv`.

These two command line options won't work on your local machine, since
they access the hardware performance counters on our server.  However,
you can run the code locally with

```
runlab --devel
```

Commit the resulting `inst_mix.csv` and `pe.csv` as `outputs/devel-inst_mix.csv` and `outputs/devel-pe.csv`. You will have to use -f to force adding a .csv file. Please make sure you only add these files.

### Test the Starter Code on the Autograder

Submit it to the autograder to confirm that it works.

### Configuration Options

Take a look at the file `config.env` in the root of the repo.  This file lets you change how the code will be built and run on the reference processor:

1. `COMPILER=gcc-9`: Use the latest version of the `gcc` compiler (version 9)
2. `OPTIMIZE=-O0` : Compile without optimizations
3. `CMD_LINE_OPTIONS=--dataset mnist` : Run the test code with the mnist dataset.
4. `#GPROF=yes` : Turn on the profiler (commented out with the `#`)

### Datasets

We will use several datasets in the course:

Name        | input dim    | categories |Description                         | URL                                                           |
------------|--------------|------------|------------------------------------|---------------------------------------------------------------|
`mnist`     | 28x28 (gray) | 10         | Handwritten digits                 | http://yann.lecun.com/exdb/mnist/                             |
`emnist`    | 28x28 (gray) | 62         | Handwritten digits and letters     | https://www.nist.gov/itl/products-and-services/emnist-dataset |
`cifar10`   | 32x32 (rgb)  | 10         | Image categorization               | https://www.cs.toronto.edu/~kriz/cifar.html                   |
`cifar100`  | 32x32 (rgb)  | 100        | Image categorization               | https://www.cs.toronto.edu/~kriz/cifar.html                   |

You can run multiple datasets consecutively by passing the `--dataset` option
multiple times.  For example, `--dataset mnist --dataset emnist` will run both
`mnist` and `emnist` datasets.

### Section. Measuring Performance

Your first task is to measure the baseline performance of the perceptron on
each of the datasets.  Modify the `cmd_line` option in `config.env` to run all the
data sets.

Commit the changes and submit the changes.

Once the autograder completes, download the zip file via the link, and open it.

If you open up `inst_mix.csv` you'll see that it's pretty hard to read:

```
GPROF,OPTIMIZE,DEVEL_MODE,STATS,dataset,training_inputs_count,nJ,runtime,insts,mem_ops,branches,uncond_branches,
no,-O0,,INST_MIX,mnist,200,1728088378,0.13515782356262207,881283776,648288017,83814492,69271166,
no,-O0,,INST_MIX,emnist,150,4785522460,0.34865498542785645,2378598263,1749969246,226119981,186974726,
```

It's in comma-separated-value format which is good for importing into
excel or Google Sheets.  (**TIP**: The easiest way to get it into a
spread sheet is to copy and paste it into the top left of a Google
Sheet. Then select `Data -> Split Text into columns`.  Voila!).

From inside your docker container, you can  make it easier to read with:

```
pretty-csv inst_mix.csv
```

You should see something like this (but longer):

```
GPROF|OPTIMIZE|DEVEL_MODE|STATS   |dataset |training_inputs_count|nJ      |runtime|insts   |mem_ops |branches|uncond_branches|
-----|--------|----------|--------|--------|---------------------|--------|-------|--------|--------|--------|---------------|
no   |-O0     |          |INST_MIX|mnist   |2e+02                |1.73e+09|0.135  |8.81e+08|6.48e+08|8.38e+07|6.93e+07       |
no   |-O0     |          |INST_MIX|emnist  |1.5e+02              |4.79e+09|0.349  |2.38e+09|1.75e+09|2.26e+08|1.87e+08       |
```

From now on, we format csv files like this in the lab write ups:

GPROF|OPTIMIZE|DEVEL_MODE|STATS   |dataset |training_inputs_count|nJ      |runtime|insts   |mem_ops |branches|uncond_branches|
-----|--------|----------|--------|--------|---------------------|--------|-------|--------|--------|--------|---------------|
no   |-O0     |          |INST_MIX|mnist   |2e+02                |1.73e+09|0.135  |8.81e+08|6.48e+08|8.38e+07|6.93e+07       |
no   |-O0     |          |INST_MIX|emnist  |1.5e+02              |4.79e+09|0.349  |2.38e+09|1.75e+09|2.26e+08|1.87e+08       |


Copy and commit `pe.csv` and `inst_mix.csv` to `outputs/baseline-pe.csv` and `outputs/baseline-inst_mix.csv`. You will have to use -f to force adding a .csv file. Please make sure you only add these files.

### Section. Enabling the Profiler

To understand why the code runs like it does, you will use the `gprof` profiler to
collect information about where all the time goes.

To do that, uncomment the `GPROF` line in `config.env` to turn on the
profiler.

Commit the change, and submit it to the autograder.  When it comes
back `pe.gprof` should look like this.  Your output will look
different, since this is the output for just one of the input datasets
(`cifar10`):

```
Flat profile:

Each sample counts as 0.01 seconds.
  %   cumulative   self              self     total           
 time   seconds   seconds    calls  ms/call  ms/call  name    
 50.00      0.27     0.27 33828720     0.00     0.00  tensor_t<double>::get(int, int, int, int)
 24.07      0.40     0.13 33828720     0.00     0.00  tensor_t<double>::operator()(int, int, int, int)
 14.82      0.48     0.08      100     0.80     2.25  fc_layer_t::activate(tensor_t<double>&)
  7.41      0.52     0.04      100     0.40     1.85  fc_layer_t::fix_weights()
  3.70      0.54     0.02      100     0.20     1.29  fc_layer_t::calc_grads(tensor_t<double> const&)
  0.00      0.54     0.00     2320     0.00     0.00  tensor_t<double>::calculate_data_size() const
  0.00      0.54     0.00     1808     0.00     0.00  tensor_t<double>::~tensor_t()
  0.00      0.54     0.00     1408     0.00     0.00  tensor_t<double>::tensor_t(tensor_t<double> const&)
  0.00      0.54     0.00     1109     0.00     0.00  tensor_t<double>::element_count() const
  0.00      0.54     0.00     1000     0.00     0.00  fc_layer_t::activator_function(double)
  0.00      0.54     0.00     1000     0.00     0.00  fc_layer_t::activator_derivative(double)
  0.00      0.54     0.00     1000     0.00     0.00  tensor_t<double>::get(int, int, int, int) const
  0.00      0.54     0.00     1000     0.00     0.00  tensor_t<double>::operator()(int, int, int, int) const
  0.00      0.54     0.00      708     0.00     0.00  test_case_t* std::__addressof<test_case_t>(test_case_t&)
  0.00      0.54     0.00      704     0.00     0.00  operator new(unsigned long, void*)
  0.00      0.54     0.00      670     0.00     0.00  test_case_t const& std::forward<test_case_t const&>(std::remove_reference<test_case_t const&>::type&)
  0.00      0.54     0.00      654     0.00     0.00  test_case_t::~test_case_t()
  0.00      0.54     0.00      604     0.00     0.00  std::vector<layer_t*, std::allocator<layer_t*> >::size() const
  0.00      0.54     0.00      596     0.00     0.00  point_t::operator==(point_t const&) const
  0.00      0.54     0.00      467     0.00     0.00  std::vector<test_case_t, std::allocator<test_case_t> >::size() const
  0.00      0.54     0.00      454     0.00     0.00  test_case_t::test_case_t(test_case_t const&)
  0.00      0.54     0.00      454     0.00     0.00  void std::_Destroy<test_case_t>(test_case_t*)
  0.00      0.54     0.00      412     0.00     0.00  tensor_t<double>::get_total_memory_size() const
  0.00      0.54     0.00      408     0.00     0.00  point_t::point_t()
  0.00      0.54     0.00      406     0.00     0.00  tensor_t<double>::tensor_t(point_t const&)
  0.00      0.54     0.00      404     0.00     0.00  __gnu_cxx::__normal_iterator<test_case_t const*, std::vector<test_case_t, std::allocator<test_case_t> > >::base() const
  0.00      0.54     0.00      400     0.00     0.00  tensor_t<double>::read(std::basic_ifstream<char, std::char_traits<char> >&)
  0.00      0.54     0.00      300     0.00     0.00  std::vector<layer_t*, std::allocator<layer_t*> >::operator[](unsigned long)
  0.00      0.54     0.00      283     0.00     0.00  std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >*
...
```

Review the slides/video from class for details of how to interpret the
profiler's output.

There are few things to look for in your output:

1. The `fc_*` functions are methods of the full-connected layer (i.e.,
a perceptron)

2. The `tensor_t<double>` funtions are methods of the tensor class
Canela uses as its main data type.  It's essentially a 3-dimensional
array of `double`.

3. Three of the top functions (`fc_layer_t::activate`,
`fc_layer_t::calc_grads`, `fc_layer_t::fix_weights`) are executed a
relatively small number of times but account for a lot of time.  In
the example above, these account for 26% of execution time.  How much
are they in your data?

4. The remaining functions in the top 10 are called many, many times,
but are very brief.  These account for 74% of execution time, in the
example.  How much are they in your data?

The source code for Canela is in `/course/CSE141pp-SimpleCNN/CNN`.  Find the
implementations of the top five functions.  You'll notice that the
frequently-executed functions, short functions are very simple (i.e.,
no loops, few or no branches), while the infrequently-executed
functions are more complex.

Unfortunately, there are not obvious ways to improve the performance
of the frequent, short functions, because they are very simple -- they
just don't do that much.

Rename `pe.gprof` to `outputs/baseline.gprof` and commit it.  Copy and commit
`pe.csv` and `inst_mix.csv` to `outputs/baseline-gprof-pe.csv` and
`outputs/baseline-gprof-inst_mix.csv`. You will have to use -f to force adding a .csv file. Please make sure you only add these files.

### Taking a Closer Look at the Code

Let's look more closely at what's going on with the frequently-called
functions.  Open up your `outputs/baseline.gprof` and page down.
There will be pages and pages of really long, messy-looking function
names that are all for functions that contribute almost nothing to
execution time.  Search for "Call graph".  You should see something
like this:

```
index % time    self  children    called     name
                                                 <spontaneous>
[1]    100.0    0.00    0.54                 main [1]
                0.00    0.54       1/1           train_model(model_t*, dataset_t&, int) [4]
                0.00    0.00       1/1           build_model(dataset_t const&) [12]
                0.00    0.00       4/4           dataset_t::~dataset_t() [513]
                0.00    0.00       3/4           std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > std::operator+
                0.00    0.00       3/9           bool std::operator==<char, std::char_traits<char>, std::allocator<char> >(std::__cxx11::basic_
                0.00    0.00       2/24          std::vector<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >
                0.00    0.00       2/3           std::vector<int, std::allocator<int> >::vector() [573]
                0.00    0.00       2/68          bool __gnu_cxx::operator!=<std::__cxx11::basic_string<char, std::char_traits<char>, std::alloc
                0.00    0.00       2/4           dataset_t::dataset_t() [512]
                0.00    0.00       2/2           dataset_t::read(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>
                0.00    0.00       2/2           dataset_t::operator=(dataset_t&&) [593]
                0.00    0.00       2/2           dataset_t::get_total_memory_size() const [599]
                0.00    0.00       2/2           dataset_t::size() const [600]
                0.00    0.00       2/2           bool __gnu_cxx::operator!=<int*, std::vector<int, std::allocator<int> > >(__gnu_cxx::__normal
                0.00    0.00       2/7           std::vector<int, std::allocator<int> >::~vector() [406]
                0.00    0.00       2/65          std::vector<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >
                0.00    0.00       1/36          std::vector<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >
                0.00    0.00       1/1           std::vector<int, std::allocator<int> >::push_back(int const&) [807]
                0.00    0.00       1/1           void archlab_add_option<int>(std::__cxx11::basic_string<char, std::char_traits<char>, std::
                0.00    0.00       1/1           void archlab_add_option<std::vector<std::__cxx11::basic_string<char, std::char_traits<char>
                0.00    0.00       1/1           std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > std::opera
                0.00    0.00       1/1           void archlab_add_option<std::vector<int, std::allocator<int> > >(std::__cxx11::basic_string<
                0.00    0.00       1/59          std::vector<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >
                0.00    0.00       1/467         std::vector<test_case_t, std::allocator<test_case_t> >::size() const [54]
...
```

This is the call graph entry for `main()`.  It appears on the second
line, which shows that its ID number is 1 (it's the first function
listed in the profile) and it took 100% of execution time.  The items
above it are places it is called from (just one, in this case), and
the lines after are functions it calls.  The "called" column shows
many of the calls `main()` accounts for.  For instance,
`std::vector<test_case_t, std::allocator<test_case_t> >::size() const`
was called 467 times and one of them is from `main()`.

Page down to search for  the function that accounts for the most
execution time in your flat profile (if your top function is one of
the `get` functions, use the next function instead).  In the
example, it's `fc_layer_t::activate`.

You'll find something like this:

```
                0.08    0.15     100/100         model_t::forward_one(tensor_t<double>&, bool) [9]
[8]     46.7    0.08    0.15     100         fc_layer_t::activate(tensor_t<double>&) [8]
                0.05    0.10 12289000/33828720     tensor_t<double>::operator()(int, int, int, int) [5]
                0.00    0.00    1100/1109        tensor_t<double>::element_count() const [43]
                0.00    0.00    1000/1000        fc_layer_t::activator_function(double) [44]
                0.00    0.00     100/100         layer_t::copy_input(tensor_t<double> const&) [103]
```

Notice anything interesting?  First, this function (and the functions
it calls), takes 46.7% of execution time -- a juicy target for
optimization!  Second, several of the short, frequently-called
functions are called many, many times by this function.  So, even
though `tensor_t<double>::operator()(int, int, int, int)` and
`fc_layer_t::activate` appeared to be independent in the flat profile,
their performance and contribution to runtime are actually deeply
intertwined.

### Section. What's the Compiler Doing?

To learn more about what's going on, let's take a look at what the compiler is
doing with the code.  Here's the source code for `fc_layer_t::activate`:

```
void activate( tensor_t<double>& in ) {
    copy_input(in);

    ...

    for ( int b = 0; b < activator_input.size.b; b++) {
        for ( int n = 0; n < activator_input.size.x; n++ ) {
            activator_input(n, 0, 0, b) = 0;
        }
    }

    for ( int b = 0; b < in.size.y; b++ ) {
        for ( int i = 0; i < in.size.x; i++ ) {
            for ( int n = 0; n < out.size.x; n++ ) {
                double in_val = in(i, b, 0);
                double weight_val = weights( i, n, 0 );
                double mul_val = in_val * weight_val;
                double acc_val = activator_input(n, 0, 0, b) + mul_val;
                activator_input(n, 0, 0, b) = acc_val;
            }
        }
    }

    for ( unsigned int n = 0; n < activator_input.element_count(); n++ ) {
        out.data[n] = activator_function( activator_input.data[n] );
    }
    ...
}
```

You'll become intimately acquainted with this code in the next lab.
For now, we can just observe a few key things about this code (Since
it's C++, function invocations can be hard to spot):

1. It's a bunch of nested loops.

2. `tensor_t<double>::operator()(int, int, int, int)` is getting invoked by `in(i, b, 0)`, `activator_input(n ,0 ,0, b)`, and `weights(i, n, 0)` which does multi-dimension access on the respective tensors.

3. `tensor_t<double>::operator()(int, int, int, int)` is also getting invoked by `activator_input(n, 0, 0, b) = acc_val`, which support assignment to the tensor `acc_val`.


That accounts for all the frequently-called functions.

Now we know why those functions are getting so much, but what does that look
like to processor?  For that, we need to see the assembly that the compiler is
generating.

You should have a `code.s` in your local directory (if you don't, do `runlab
--devel` to regenerate it).  It contains the assembly for everything in
`code.cpp`, which includes `fc_layer_t::activate()`.  The file is really
long -- over 10,000 lines -- so we'll have to search for what we need.

Try searching for `fc_layer_t::activate` -- Any luck?  I thought not.  Try
searching for `activate` instead.  You should find something like this:

```
.LLSDACSE5207:
        .section        .text._ZN7layer_t10copy_inputERK8tensor_tIdE,"axG",@progbits,_ZN7layer_t10copy_inputERK8tensor_tIdE,comdat
        .size   _ZN7layer_t10copy_inputERK8tensor_tIdE, .-_ZN7layer_t10copy_inputERK8tensor_tIdE
        .section        .text._ZN10fc_layer_t8activateER8tensor_tIdE,"axG",@progbits,_ZN10fc_layer_t8activateER8tensor_tIdE,comdat
        .align 2
        .p2align 4
        .weak   _ZN10fc_layer_t8activateER8tensor_tIdE
        .type   _ZN10fc_layer_t8activateER8tensor_tIdE, @function
_ZN10fc_layer_t8activateER8tensor_tIdE:
.LFB5244:
        .cfi_startproc
        pushq   %rbp
        .cfi_def_cfa_offset 16
        .cfi_offset 6, -16
```

It turns out that `__ZN10fc_layer_t8activateER8tensor_tIdE` is the
"mangled" name for `fc_layer_t::activate(tensor_t<double>&)`.
Mangling is necessary for C++ for a bunch of reasons, notably
templates and function/operator overloading, and because the assembler
and linker don't like weird punctuation in symbol names.

The exact algorithm for mangling is compiler-dependent.  Fortunately,
`g++` provides a demangling utility called `c++filt` that will clean
this up for us.  Open the file again with

```
c++filt < code.s | less
```

Or created a filtered version with 

```
c++filt < code.s > code-demangled.s
```

Commit the resulting code as `outputs/baseline-demangled.s` and open it. You will have to use -f to force adding a .s file. Please make sure you only add these files.

Now searching for `fc_layer_t::activate` will give you:

```
        .weak   fc_layer_t::activate(tensor_t<double>&)
        .type   fc_layer_t::activate(tensor_t<double>&), @function
fc_layer_t::activate(tensor_t<double>&):
.LFB5244:
        .cfi_startproc
        pushq   %rbp
        .cfi_def_cfa_offset 16
        .cfi_offset 6, -16
        movq    %rsp, %rbp
        .cfi_def_cfa_register 6
        pushq   %rbx
        subq    $136, %rsp
        .cfi_offset 3, -24
1:      call    mcount
        movq    %rdi, -136(%rbp)
        movq    %rsi, -144(%rbp)
...
```

There's a lot here, but you don't need to understand it in detail to get some idea of what's going.  Here's some key facts that will make the assembly more comprehensible:

1.  Lines that end in `:` are _labels_ - just named places in the code (e.g., the beginnings of functions)

2.  Labels that begin with `.` are generated by the compiler and are used as branch targets.

3.  Non-labels that start with `.` are assembler directives, not instructions.  You can generally ignore them.

4.  `call mcount` is part of `gprof`, those calls won't be there if you compiled without the profiler.

If you scroll down a bit until you find the assembly code for the body of the inner loop. You'll know it when you find because it'll have `call` instructions to invoke the frequently-called functions.  Here it is:


```
...
.L270:
        movq    -136(%rbp), %rax
        movl    32(%rax), %eax
        cmpl    %eax, -104(%rbp)
        jge     .L269
        movl    -112(%rbp), %edx
        movl    -108(%rbp), %esi
        movq    -144(%rbp), %rax
        movl    $0, %r8d
        movl    $0, %ecx
        movq    %rax, %rdi
        call    tensor_t<double>::operator()(int, int, int, int)
        movsd   (%rax), %xmm0
        movsd   %xmm0, -96(%rbp)
        movq    -136(%rbp), %rax
        leaq    104(%rax), %rdi
        movl    -104(%rbp), %edx
        movl    -108(%rbp), %eax
        movl    $0, %r8d
        movl    $0, %ecx
        movl    %eax, %esi
        call    tensor_t<double>::operator()(int, int, int, int)
        movsd   (%rax), %xmm0
        movsd   %xmm0, -88(%rbp)
        movsd   -96(%rbp), %xmm0
        mulsd   -88(%rbp), %xmm0
        movsd   %xmm0, -80(%rbp)
        movq    -136(%rbp), %rax
        leaq    80(%rax), %rdi
        movl    -112(%rbp), %edx
        movl    -104(%rbp), %eax
        movl    %edx, %r8d
        movl    $0, %ecx
        movl    $0, %edx
        movl    %eax, %esi
        call    tensor_t<double>::operator()(int, int, int, int)
        movsd   (%rax), %xmm0
        movsd   -80(%rbp), %xmm1
        addsd   %xmm1, %xmm0
        movsd   %xmm0, -72(%rbp)
        movq    -136(%rbp), %rax
        leaq    80(%rax), %rdi
        movl    -112(%rbp), %edx
        movl    -104(%rbp), %eax
        movl    %edx, %r8d
        movl    $0, %ecx
        movl    $0, %edx
        movl    %eax, %esi
        call    tensor_t<double>::operator()(int, int, int, int)
        movsd   -72(%rbp), %xmm0
        movsd   %xmm0, (%rax)
        addl    $1, -104(%rbp)
        jmp     .L270
...
```

There are few things to notice about this code:

1.  There are a lot of `mov` instructions with `(` and `)` in the arguments.  In x86 assembly, these are memory access instructions.

2.  There are quite few `call` instructions.

3.  There are a several instructions accessing `xmm` registers.  These are floating point operations.

One of those calls is to `tensor_t<double>::operator()(int, int, int, int)`.
Take a look at the implementation in the source and compare it to the
assembly (about 18 instructions):

```
tensor_t<double>::operator()(int, int, int, int):
.LFB5804:
        .cfi_startproc
        pushq   %rbp
        .cfi_def_cfa_offset 16
        .cfi_offset 6, -16
        movq    %rsp, %rbp
        .cfi_def_cfa_register 6
        subq    $32, %rsp
1:      call    mcount
        movq    %rdi, -8(%rbp)
        movl    %esi, -12(%rbp)
        movl    %edx, -16(%rbp)
        movl    %ecx, -20(%rbp)
        movl    %r8d, -24(%rbp)
        movl    -24(%rbp), %edi
        movl    -20(%rbp), %ecx
        movl    -16(%rbp), %edx
        movl    -12(%rbp), %esi
        movq    -8(%rbp), %rax
        movl    %edi, %r8d
        movq    %rax, %rdi
        call    tensor_t<double>::get(int, int, int, int)
        leave
        .cfi_def_cfa 7, 8
        ret
        .cfi_endproc
```

Note the large number of memory operations (i.e., instructions with parentheses).

This function calls `tensor_t<double>::get(int, int, int, int)` which looks like this (about 37 instructions):

```
.LFB5915:
        .cfi_startproc
        pushq   %rbp
        .cfi_def_cfa_offset 16
        .cfi_offset 6, -16
        movq    %rsp, %rbp
        .cfi_def_cfa_register 6
        subq    $24, %rsp
1:      call    mcount
        movq    %rdi, -8(%rbp)
        movl    %esi, -12(%rbp)
        movl    %edx, -16(%rbp)
        movl    %ecx, -20(%rbp)
        movl    %r8d, -24(%rbp)
        movq    -8(%rbp), %rax
        movq    16(%rax), %rdx
        movq    -8(%rbp), %rax
        movl    (%rax), %ecx
        movq    -8(%rbp), %rax
        movl    4(%rax), %eax
        imull   %eax, %ecx
        movq    -8(%rbp), %rax
        movl    8(%rax), %eax
        imull   %ecx, %eax
        imull   -24(%rbp), %eax
        movl    %eax, %ecx
        movq    -8(%rbp), %rax
        movl    (%rax), %esi
        movq    -8(%rbp), %rax
        movl    4(%rax), %eax
        imull   %esi, %eax
        imull   -20(%rbp), %eax
        addl    %eax, %ecx
        movq    -8(%rbp), %rax
        movl    (%rax), %eax
        imull   -16(%rbp), %eax
        addl    %eax, %ecx
        movl    -12(%rbp), %eax
        addl    %ecx, %eax
        cltq
        salq    $3, %rax
        addq    %rdx, %rax
        leave
        .cfi_def_cfa 7, 8
        ret
        .cfi_endproc
```

Again, lots of memory access.  So the expression `in( i, b, 0 )` results in the
execution of over 55 instructions, about half of which are memory accesses.

Check the source code for `tensor_t<double>::get(int, int, int, int)` and make sure
you understand it.  The heart of `in( i, b, 0 )` is a handful of multiplies and
additions.  If you look carefully, you can see them near the end of the listing
above.  Most of the remaining instructions are just overhead related to calling
the functions and moving data on and off the stack.

### Section. Looking at the Performance Counters

The analysis of the profiling data and the compiler output suggests that

1. Small, frequently called functions are a performance problem, and

2. The compiler's baseline implementation of these functions and how they are called are inefficient.

We can bolster this analysis further by looking at the hardware
performance counters while our test code is running.  You've already collected
this data in `outputs/baseline-inst_mix.csv`.  Here's an example:

GPROF|OPTIMIZE|DEVEL_MODE|STATS   |dataset |training_inputs_count|nJ      |runtime|insts   |mem_ops |branches|uncond_branches|
-----|--------|----------|--------|--------|---------------------|--------|-------|--------|--------|--------|---------------|
no   |-O0     |          |INST_MIX|mnist   |2e+02                |1.73e+09|0.135  |8.81e+08|6.48e+08|8.38e+07|6.93e+07       |
no   |-O0     |          |INST_MIX|emnist  |1.5e+02              |4.79e+09|0.349  |2.38e+09|1.75e+09|2.26e+08|1.87e+08       |

For your data (and all the data sets), calculate the percentage
of total instructions that are memory operations, branches, and unconditional
branches.

* mnist
* emnist
* cifar10
* cifar100

Also calculate the number of memory operations, branches, and unconditional
branches *per training input*.

The output from your test contains this line and others like it: 

```
Total 1: 2.9e+02 kB
```

That gives the total memory required for the model.  Compare this
number to the number of memory operations performed per training
input.  

### Section. Asking the Compiler to Do More

Clearly, this code is doing more work than it needs to, but how should we fix it?

There are two main sources of inefficiency in the code above.

1. There are a lot of memory accesses.

2. Calling small functions (that sometimes, in turn, call small
functions) leads to inefficiency.

Both of these are very common performance problems.  The second is
especially common in object-oriented code, since its goal is, in
large measure, to facilitate code reuse, which tends to lead to lots
of function calls to small functions.  (Your CSE11 or 8a/8b professors
added fuel to the fire by suggesting you break programs up into
small, easy-to-understand pieces.)

Fixing these problems in the source code is probably possible, but the changes
required to the code would be complex, error-prone, and probalby result in code
that was unmaintainable.

Instead, we can just ask the compiler to fix it!

Set `OPTIMIZE=-O3` and leave `gprof` enabled in `config.env`, commit and resubmit
to the autograder.

Commit the resulting csv files as `outputs/optimized-gprof-pe.csv` and  `outputs/optimized-gprof-inst_mix.csv`. You will have to use -f to force adding a .csv file. Please make sure you only add these files.

**Note:** If git complains about adding a file listed in `.gitignore`
  you can either 1) edit your .gitignore or 2) use `-f` to force the
  add.

Demangle `code.s` and take a look at the inner loop body of the new
version of `fc_layer_t::activate`:

```
...
        testl   %ebx, %ebx
        jle     .L319 
        movslq  -168(%rbp), %rax
        leal    -1(%rbx), %edx
        movq    16(%rdi), %rdi
        movq    $0, -128(%rbp)
        movl    %edx, -104(%rbp)
        salq    $3, %rax
        movq    %rdi, -56(%rbp)
        movq    %rax, -200(%rbp)
        movslq  %ebx, %rax
        leaq    0(,%rax,8), %rcx
        subq    $1, %rax
        movl    $0, -132(%rbp)
        movq    %rcx, -144(%rbp)
        leaq    0(,%rax,8), %rcx
        movq    %rcx, -152(%rbp)
        leaq    0(,%rdx,8), %rcx
        movq    %rcx, -160(%rbp)
        movq    -216(%rbp), %rcx
        movl    $0, -136(%rbp)
        movslq  104(%rcx), %rdx
        movq    96(%rcx), %r9
        movl    $0, -112(%rbp)
        movq    120(%rcx), %rdi
        xorl    %ecx, %ecx
        leaq    0(,%rdx,8), %r13
...
```

It looks a great deal better!  Many fewer memory accesses and no function
calls!  

### Section. Measuring Actual Performance

So far, we have been running the code with the profiler enabled, but `gprof`
adds some overhead.  To measure the real performance, disable `gprof` in
`config.env` and resubmit.

Save the resulting csv file as `outputs/optimized{-pe,-inst_mix}.csv`.

### Reasoning About Performance

Pick the functions that accounted for the largest fraction of
execution time in your optimized gprof output.  We will call this your
"hot function".

Modify your `config.env` to re-enable gprof and run just one workload.
Submit it, and save the resulting gprof output.  Repeat this with the
other workloads.

While that's running, examine the the code for your function in the
Canela source code.  What is the O() (i.e., Big-O) complexity of this
function?  In your O() expression use 'm' as input size and 'n' as the
output size.  The input size is the total number of inputs.  For our
datasets, this is product of the width, height, and depth of the
image.  Depth is 1 for grayscale and 3 for RGB.  'n' is the number of
categories.  The necessary information is given in the table above.

Using your O() expression estimate the runtime for each dataset
relative to mnist.  For instance:

| dataset  | m    | n   | relative-m     | relative-n |
|----------|------|-----|----------------|------------|
| mnist    | 784  | 10  | 1 = 784/784    | 1 = 10/10  |
| cifar100 | 3072 | 100 | 3.9 = 3072/784 | 10 = 100/10|

You can then estimate execution time of `cifar100` *relative to*
`mnist` using the relative value.  For instance, if you estimate that
your hot functions is O(n\*m), then the relative execution time for
`cifar100` is 3.9*10=39.

Follow the instructions in the lab write up to analyze this data.

### Changing the Clock Rate and Measuring Power

First, we no longer need to use gprof, so lets comment it out.
```
#GPROF=yes
```

If inside `pe.csv` you'll see a `MHz` column.  It shows the clock rate
your experiments have been running at.  You can control the clock rate
with the `--MHz` option in `CMD_LINE_ARGS` option in your
`config.env`.  For now, just add `--MHz 900`.

To speed things up, we'll just do one data set: `cifar100`.  Adjust
your `config.env` accordingly.

We will also measure energy consumption.  To do this, you'll need to
add a new performance counter.  Take a look in
`papi_native_avial.txt`.  It lists all of the performance counters
available on our processor.  As you can see, there are many of them.

The counter we want is part of Intel's Running Average Power Limt
(RAPL) interface, and it measures the number of nanojoules used by the
processor package (which includes the whole chip -- all the cores and
peripheral circuits).  It is called `rapl:::PACKAGE_ENERGY:PACKAGE0`.

To measure it add this to your `CMD_LINE_ARGS` in `config.env`:

```
--stat nJ=rapl:::PACKAGE_ENERGY:PACKAGE0
```

Let's break that down:

* `--stat` tells your executable to measure a performance counter.
* `nJ=` sets the column label that will appear in the CSV file.
* `rapl:::PACKAGE_ENERGY:PACKAGE0` is the counter you want to measure.

We will also measure power.  There is no performance counter for
power, but we can compute it from energy and execution time.  To
compute power (in Watts), add this to `CMD_LINE_ARGS` (Why did I add
`/1e9`?):

```
--calc W=nJ/runtime/1e9
```

* `--calc` says you are adding a computed column to the CSV file.

* `W=` is the name of the column

* `nj/runtime/1e9` is the expression.  It can be any valid Python
  expression.  You can use column names as variables.

You should end up with something in your `config.env` like:

``` 
CMD_LINE_ARGS="--dataset cifar100 --MHz 900 --stat nJ=rapl:::PACKAGE_ENERGY:PACKAGE0 --calc W=nJ/runtime/1e9"

```

After you've made all these changes, commit and submit.

Check `pe.csv` to verify that the clock rate changed.  Copy (but you don't need to
commit) `pe.csv` to something like `pe-900.csv`.  You'll have
something like this (some columns and rows have been removed):

| nJ     |inst_count|runtime|MHz    |cycles |IPC | W  | 
|--------|----------|-------|-------|-------|----|----|
|2.25e+09|8.27e+08  |0.456  |9e+02  |4.1e+08|2.02| 4.9|

Here's what the fields mean:

* `nJ` -- nanojoules consumed by the entire processor die.
* `inst_count` -- Instruction count
* `runtime` -- execution time (or latency)
* `MHz` -- The value of the clock rate parameter you passed to the tool.
* `cycles` -- number of actual (i.e., not reference) clock cycles.
* `IPC` -- Instructions per cycle.
* `W` -- How many Watts the processor die is consuming.

Add `--MHz 1000`,  `--MHz 1100`,  up to `--MHz 2000` and rerun.

Combine the resulting `pe.csv` files to `outputs/pe-clockrate.csv` and
commit `outputs/pe-clockrate.csv`. You will have to use -f to force
adding a .csv file. Please make sure you only add these files.  The
resulting file should have 12 rows of data and a header.

Load `outputs/pe-clockrate.csv` into a spreadsheet. (**TIP**: The
easiest thing to do is copy it's contents and paste it into a Google
Sheet. Then select `Data -> Split Text into columns`).

Note that, as expected, `inst_count`, `cycles`, and `IPC` don't change
when we changed the clock speed.

<!---
### Section. Memory Accesses with Moneta

Moneta is a tool to generate and visualize billions of memory accesses with this project built on PIN and HDF5. Pin is the instrumentation tool that we used to read and interpret memory accesses at runtime. It runs in the background whenever you generate traces and produces the trace files for your program.

Moneta uses tags to identify the memory accesses. The code in `moneta_code.cpp` is `code.cpp` modified with tags. 

To build the modified code, run `bash moneta_build.sh` and it will generate the executable `code.exe`. 

Moneta is installed already in your docker container. Run the following command to start the tool.

```
moneta
``` 

It will start a Jupyter notebook which you can access from your browser. Open the `Moneta.ipynb` file.

Enter the inputs as shown in the image below and click on `Generate Trace`. This will run the executable and create the trace file. 

![Moneta Details](./moneta-details.png?raw=true)

Once it is completed, select the trace from the column on the right and click on `Load Trace` to load the memory accesses. You should see the tags and some accesses visible on the graph.

![Moneta UI](./moneta-UI.png?raw=true)

Explore the tool to see the different memory access patterns for the Input, Weight and Output tensors of the perceptron model. Each tag comes with a button which on hover shows detailed information of the tag including accesses and hit rate. You can click on the button to zoom in to just the tag you want to see.
-->
## Turn in Your Work

Submit your code repo and completed worksheet via their respective
assignments on gradescope.  You can submit the code portion as many
times as you like, but the worksheet only once.


