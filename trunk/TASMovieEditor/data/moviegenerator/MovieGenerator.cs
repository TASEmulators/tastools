using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;

using System.Resources;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Windows.Forms;

using MovieSplicer.Data;
using MovieSplicer.Data.Formats;
using MovieSplicer.Components;

using MovieSplicer.Data.moviegenerator;

namespace MovieSplicer.Data.moviegenerator
{
    /// <summary>
    /// Contains the frames as appear on th emovie file that have to be kept
    /// because we'll be adding blank frames in between
    /// </summary>
    public struct FrameWithInput
    {
        public int newframeposition;
        public int originalframeposition;
        public MovieSplicer.Data.TASMovieInput[] Input;
        public int total_input;
        
        public FrameWithInput(int number_inputs)
        {
            if (number_inputs < 1) number_inputs = 7;
            Input = new MovieSplicer.Data.TASMovieInput[number_inputs];
            for( int i = 0; i < number_inputs; i++ )
            {
                Input[i] = new MovieSplicer.Data.TASMovieInput();
            }
            total_input = 0;
            newframeposition = 0;
            originalframeposition = 0;
        }
    }

    /// <summary>
    /// Contains the combination data
    /// 
    /// </summary>
    public struct Combination
    {
        public enum Actions { INSERT, REMOVE, REPLACE, INSERTSUBCOMBINATION };

        public int total_values;
        public int value;

        public Actions action;
        public int frame;
    }


    class MovieGenerator
    {
        /// <summary>
        /// Movie to process.
        /// 
        /// </summary> 
        public MovieSplicer.Data.TASMovie movie;


///////////////////////////////////////////////////////////////
///
///	GENERAL


        /// <summary>
        /// When doing automated input searches (more on that later)
        /// the program has to figure out when an input is being held
        /// (and treat it like a single entry) or not.
        ///
        /// An input will be considered a single entry until this many
        /// frames pass. After this, if the input is still held, then
        /// it will considered a new entry.
        ///
        /// Note that if a button is held but the input changes,
        /// then they will be considered separate entries no matter
        /// what. ie. >,>,>,>B,>,>,> here we have 3 entries.
        ///
        /// Try not to place a bigger number unless you actually have a
        /// held input like that, because this is NOT using ArrayLists
        /// and as such will use a lot of memory with big numbers.
        /// Which is not as bad as the following if you do have inputs
        /// held for long.
        ///
        /// If you have a long held input, and you keep a low number
        /// here, then you will greatly increase the number
        /// of combinations possible!, so keep this value updated!
        ///
        /// Default empty = 7
        /// </summary>
        public int heldinput_max;


        /// <summary>
        /// When generating the combinations you can easily reach
        /// huge numbers, billions, trillions.
        ///
        /// Your pc won't stand that many files, here you can tell
        /// the program how many files you want created as maximum.
        ///
        /// The program will create a "savestate" file of where it
        /// stopped, so next time you load the program you can
        /// continue generating movies.
        ///
        /// Default 25000
        /// </summary>
        public int maxcombinations_persave;


///////////////////////////////////////////////////////////////
///
///	FRAME INSERTION
///
///	ie. when you got a movie where you have to wait
///	'n' frames to change what a monster drops, but
///	calculate that this would take you 10 years to
///	test manually
///

/// <summary>
/// Should we do frame insertion?
/// 1 = yes
/// 0 = no
/// Default 0
/// </summary>
public bool frameinsert_do;

/// <summary>
/// Generator will start by just adding this many frames.
/// When finished, it will progress to the next level
/// until it reaches "frameinsert_max"
///
/// Advised to change both min and max to the same number
/// so you keep track of your search.
/// 
/// If you set both to zero, then... you won't insert frames.
/// </summary>
public int frameinsert_min;
/// <summary>
/// Generator will end up adding this many frames.
/// 
/// </summary> 
public int frameinsert_max;


/// <summary>
/// Comma separated list of frames where the frameinsertion
/// will take place. If empty, the program will use
/// frameinsert_framestart and frameinsert_frameend
/// to find inputs in the movie and use them instead.
///
/// If on your movie frame 2000 corresponds with the input S
/// frame 2001 with the input B, and frame 2002 with A
/// and you are type 2001 here, and you are inserting one >
/// 
/// frame 2000 stays being S
/// frame 2001 becomes >
/// frame 2002 is B which was pushed
/// frame 2003 is A which was pushed
///
/// </summary>
//frameinsert_onframes 5,10,3,1000,0
public int[] frameinsert_onframes;
        public string frameinsert_onframes_original;

/// <summary>
/// When you do not define what frames to use, the program
/// will perform an "Input Search".
///
/// It will read all inputs on your movie, and keep track
/// of their frames to fill "frameinsert_onframes".
///
/// Input search starts here. Frames before this are ignored.
/// If this is empty set to 0 (start of the movie).
/// </summary> 
public int frameinsert_framestart;
/// <summary>
/// Input search ends with this frame.
/// If this is empty set to movie lenght minus 1 (end of movie)
/// </summary>
//frameinsert_frameend 1000
public int frameinsert_frameend;

/// <summary>
/// Comma separated input combinations (as defined by tas-movie-editor)
/// that the user wants to be used.
/// * "i" is used for idle frame (no input)
/// * "Z" is used for select button.
/// ie. "i","^", ">", "AS", "<>", "ABSZ", etc
/// 
/// The user can define to use strictly the combinations he types,
/// or turn on frameinsert_combineinputs so the program generates
/// all combinations given the base they give
/// 
/// </summary>
//frameinsert_inputs i,^,v,>,<
//frameinsert_inputs A,B,S,Z
//frameinsert_inputs i,^,v,>,<,^>,<^,<v,v> to be used with frameinsert_combineinputs=1
//etc
//This will only insert idle frames.
public string[] frameinsert_inputs;
        public string frameinsert_inputs_original;
/// <summary>
/// 1 = Perform all combinations of the inputs defined
/// in "frameinsert_inputs" and replace the variable with
/// the combinations generated.
/// 
/// 0 = use frameinsert_inputs as-is.
/// </summary>
public bool frameinsert_combineinputs;

/// <summary>
/// When inserting frames, these are inserted on
/// the frames defined on "frameinsert_onframes".
/// We know that the contents of the movie get pushed,
/// but we get a problem, if we insert a frame at
/// the beginning of the movie...
/// Should insertions on other frames be pushed or stay
/// in place?
/// 
/// Let's say we want to insert on frames 10 and 20,
/// and on the current generation we will insert 8 frames.
/// This generates a lot of combinations, on the current
/// combination we will insert five frames on frame 10
/// and three frames frame 20.
/// 
/// 0 = Frames defined will be absolute. Will be used as defined.
/// Wont get pushed by insertions. So:
/// 
/// Five frames will get inserted on frame 10, and three frames
/// will get inserted on frame 20. If there was an input 
/// on frame 15 which got pushed to frame 20 then it will be
/// overwritten by the insertion at frame 20.
///
///  
/// 1 = The frames where insertion is to take place
/// will be relative. They get pushed by insertions. So:
///
/// We insert five on frame 10, and three on frame 25, NOT 20.
/// You never lose movie input this way.
///
/// "On remove" will also push this list when there is a removal
/// of frames. So if we removed 2 frames on frame 15...
/// We would insert on frame 23, not 25, not 20.
///
/// </summary>
public bool frameinsert_pushframelist_oninsert;
public bool frameinsert_pushframelist_onremove;

///
///	END
///////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////
///
///	INPUT/FRAME REPLACEMENT
///
///	ie. When a drop changes based on your
///	button presses on the last ten frames,
///	but there are 500 possible input
///	combinations per frame!!
///

/// <summary>
/// Should we do input replacement?
/// 1 = yes
/// 0 = no
/// Default 0
/// </summary>
public bool inputreplace_do;

/// <summary>
/// Comma separated list of frames where the inputreplace
/// will take place. If empty, the program will use
/// inputreplace_framestart and inputreplace_frameend
/// and replace all inputs found within.
/// 
/// If on your movie frame 2000 corresponds with the input S
/// frame 2001 with the input B, and frame 2002 with A
/// and you are type 2001 here, and you are replacing with >
/// 
/// frame 2000 stays being S
/// frame 2001 becomes >, B is lost
/// frame 2002 stays being A
/// 
/// </summary>
public int[] inputreplace_onframes;
        public string inputreplace_onframes_original;
/// <summary>
/// Input search starts with this frame
/// Used only when inputreplace_onframes is empty
/// If this is empty set to 0
///
/// If inputreplace_includeidleframes_onsearch is on
/// then it will also consider idle frames.
/// </summary> 
public int inputreplace_framestart;
/// <summary>
/// Input search ends with this frame
/// Used only when inputreplace_onframes is empty
/// If this is empty set to movie lenght-1
/// </summary>
public int inputreplace_frameend;
/// <summary>
/// Input search will include all frames
/// not only inputs but also idle frames.
/// If this is empty set to false.
/// </summary>
public bool inputreplace_includeidleframes_onsearch;
/// <summary>
/// These are the input combinations (as defined by tas-movie-editor)
/// that the user wants to be used.
/// ie. "","^", ">", "AS", "<>", "ABSZ", etc
/// 
/// The user can define to use strictly the combinations he types,
/// or turn on inputreplace_combineinputs so the program generates
/// all combinations given the base they give
/// 
/// The config has these as a single string comma separated,
/// "idle" is represented by "i".
/// </summary>
public string[] inputreplace_inputs;
        public string inputreplace_inputs_original;
/// <summary>
/// 1 = combine inputs defined in inputreplace_inputs
/// and replace the variable with the combinations generated.
/// 
/// 0 = use inputreplace_inputs as is
/// </summary>
public bool inputreplace_combineinputs;

/// <summary>
/// Lets say we not only replace but also insert frames on
/// frames 10 and 20, and remove on frame 15. We want to do
/// replacement on frame 13.
///
/// On the current combination we will insert 5 frames
/// with input on the first, and 3 frames on the second.
/// We will remove 2 frames.
///
/// 0 = The frames where replacement is to take place
/// will be absolute, will be used as defined. So:
///
/// We insert five on frame 10, and three on frame 25.
/// Removal gets pushed five, so we remove 2 on frame 
/// 20, in other words, the contents of the initial
/// frame 15 are now on frame 18.
///
/// Now we replace frame 13.
/// 
/// 1 = The frames where replacement is to take place
/// will be relative to insertions and removals. So:
/// 
/// After insertions and removals... the replacement of
/// frame 13 gets pushed five. So we replace frame 18.
/// 
/// </summary>
public bool inputreplace_pushframelist_oninsert;
public bool inputreplace_pushframelist_onremove;

///
///	END
///////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////
///
///	FRAME REMOVAL
///
///	ie. When you got a so called "optimal"
///	movie, and want to check if you really
///	didn't waste frames.
///

/// <summary>
/// Should we do frame removal?
/// 1 = yes
/// 0 = no
/// Default 0
/// </summary>
public bool frameremove_do;

/// <summary>
/// Generator will start by just removing this many frames.
/// When finished, it will progress to the next level
/// until it reaches "frameremove_max"
///
/// Advised to change both min and max to the same number
/// so you keep track of your search.
/// 
/// If you set both to zero, then... you won't remove frames.
/// </summary>
public int frameremove_min;
/// <summary>
/// Generator will end up removing this many frames.
/// 
/// </summary> 
public int frameremove_max;


/// <summary>
/// Comma separated list of frames where the frameremoval
/// will take place. If empty, the program will use
/// frameremove_framestart and frameremove_frameend
/// to find inputs in the movie and use them instead.
///
/// If on your movie frame 2000 corresponds with the input S
/// frame 2001 with the input B, and frame 2002 with A
/// and you are type 2001 here, and you remove one frame
/// 
/// frame 2000 is no longer S, it is B
/// frame 2001 is A
///
/// In other words removal is slightly tricky, it doesn't
/// remove the frame you type, but the frames before.
/// Imagine you are using the backspace key, not delete.
/// </summary>
//frameremove_onframes 5,10,3,1000,0
public int[] frameremove_onframes;
        public string frameremove_onframes_original;

/// <summary>
/// When you do not define what frames to use, the program
/// will perform an "Input Search".
///
/// It will read all inputs on your movie, and keep track
/// of their frames to fill "frameremove_onframes".
///
/// If frameremove_inputs is defined then it will only consider
/// those frames that have an input equal to one of them.
/// Otherwise it will include any input (in other words
/// everything except idle frames).
///
/// Input search starts here. Frames before this are ignored.
/// If this is empty set to 0 (start of the movie).
/// </summary> 
public int frameremove_framestart;
/// <summary>
/// Input search ends with this frame.
/// If this is empty set to movie lenght minus 1 (end of movie)
/// </summary>
//frameremove_frameend 1000
public int frameremove_frameend;

/// <summary>
/// These are the input combinations (as defined by tas-movie-editor)
/// that the user wants to be removed!
/// ie. "","^", ">", "AS", "<>", "ABSZ", etc
/// 
/// The user can define to use strictly the combinations he types,
/// or turn on frameremove_combineinputs so the program generates
/// all combinations given the base they give
/// 
/// The config has these as a single string comma separated,
/// "idle" is represented by "i".
///
/// The input combinations defined here will be the ones
/// searched while doing the Input Search.
///
/// </summary>
public string[] frameremove_inputs;
        private string frameremove_inputs_original;
/// <summary>
/// 1 = combine inputs defined in frameremove_inputs
/// and replace the variable with the combinations generated.
/// 
/// 0 = use inputreplace_inputs as is
///
/// Default 1
/// </summary>
public bool frameremove_combineinputs;

/// <summary>
/// Lets say we not only remove but also insert frames
/// on frame 10, and on the current combination
/// we will insert 5 frames with input.
/// We also want to do remove two frames on frame 13.
///
/// 0 = The removal frame lists are absolute, will be used
/// as defined. So:
/// Five frames are inserted on frame 10, and two removed
/// on frame 13.
/// 
/// 1 = The frames where removal is to take place
/// are relative. So:
/// Five frames are inserted on frame 10, and two removed
/// on frame 18. This is default behavior.
/// 
/// </summary>
public bool frameremove_pushframelist_oninsert;
public bool frameremove_pushframelist_onremove;

///
///	END
///////////////////////////////////////////////////////////////


        /// <summary>
        /// The current generation: 
        /// how many idle frames is inserting?
        /// </summary>
        private int frameinsert_current;
        /// <summary>
        /// The current generation: 
        /// how many idle frames is inserting?
        /// </summary>
        private int frameremove_current;

        /// <summary>
        /// Loop's "i"
        ///
        /// </summary>
        private int frameinsert_iteration;
        private int frameremove_iteration;

        private int frameinsert_totalframesinsertedsofar;
        private int frameremove_totalframesremovedsofar;

        /// <summary>
        /// Go to this or next loop
        /// and add one to the insert subcombination
        /// </summary>
        private bool addonemoreinsertsub;
        /// <summary>
        /// Go to this or next loop
        /// and add one to the replace combination
        /// </summary>
        private bool addonemorereplace;

        /// <summary>
        /// Starting this frame, data will be generated
        /// so remember to clear it between combinations.
        /// </summary>
        private int movieheader_endindex;
        
        /// <summary>
        /// Each int on this array represents the state of one
        /// of the combination parts. Whether insertion, or 
        /// replacement.
        /// 
        /// As a whole, this represents a single combination, a single movie.
        /// So it gets saved when outputting combinations in segments.
        /// </summary>
        private Combination[] insertCombinations;
        private Combination[] removeCombinations;
        private Combination[] replaceCombinations;
        private Combination[] insertSubCombinations;

        private string insert_combinations_restore;
        private string remove_combinations_restore;
        private string replace_combinations_restore;
        private string insert_subcombinations_restore;



        /// <summary>
        /// True when the generator should stop running
        /// despite its current state. (Save if not finished).
        /// </summary>
        public bool finishrunning;


        /// <summary>
        /// The combinations get stored here before saving.
        /// 
        /// </summary>
        MovieSplicer.Data.TASMovieInput[] baseinput;

        /// <summary>
        /// The input units found on the movie.
        /// Uses heldinput_max as defined.
        /// 
        /// It gets used when building the baseinput before saving.
        /// </summary>
        FrameWithInput[] framesWithInput;

        /// <summary>
        /// Numeric name of the last movie saved.
        /// 
        /// </summary>
        public int lastmoviesaved_index;


        /// <summary>
        /// When equals "maxcombinations_persave"
        /// stop generating and save session.
        /// </summary>
        private int numbermoviessavedthissession = 0;

        private bool restored_fromsave;


        public MovieGenerator()
        {
            InitData();
        }

        public void InitData()
        {
            movie = null;

            heldinput_max = 7;
            maxcombinations_persave = 25000;

            frameinsert_do = false;
            frameinsert_min = 0;
            frameinsert_max = 0;
            frameinsert_onframes = new int[0];
            frameinsert_onframes_original = "";
            frameinsert_framestart = 0;
            frameinsert_frameend = -1;
            frameinsert_inputs = null;
            frameinsert_inputs_original = "";
            frameinsert_combineinputs = true;
            frameinsert_pushframelist_oninsert = true;
            frameinsert_pushframelist_onremove = true;


            inputreplace_do = false;
            inputreplace_onframes = new int[0];
            inputreplace_onframes_original = "";
            inputreplace_framestart = 0;
            inputreplace_frameend = -1;
            inputreplace_includeidleframes_onsearch = false;
            inputreplace_inputs = null;
            inputreplace_inputs_original = "";
            inputreplace_combineinputs = true;
            inputreplace_pushframelist_oninsert = true;
            inputreplace_pushframelist_onremove = true;


            frameremove_do = false;
            frameremove_min = 0;
            frameremove_max = 0;
            frameremove_onframes = new int[0];
            frameremove_onframes_original = "";
            frameremove_framestart = 0;
            frameremove_frameend = -1;
            frameremove_inputs = null;
            frameremove_inputs_original = "";
            frameremove_combineinputs = true;
            frameremove_pushframelist_oninsert = true;
            frameremove_pushframelist_onremove = true;


            frameinsert_current = -1;
            frameremove_current = -1;
            frameinsert_iteration = -1;
            frameremove_iteration = -1;
            frameinsert_totalframesinsertedsofar = 0;
            frameremove_totalframesremovedsofar = 0;
            addonemoreinsertsub = false;
            addonemorereplace = false;

            insertCombinations = null;
            removeCombinations = null;
            replaceCombinations = null;
            insertSubCombinations = null;
            insert_combinations_restore = "";
            remove_combinations_restore = "";
            replace_combinations_restore = "";
            insert_subcombinations_restore = "";

            finishrunning = false;

            movieheader_endindex = 0;

            lastmoviesaved_index = 0;
            numbermoviessavedthissession = 0;
            restored_fromsave = false;
        }

        public void LoadConfig()
        {
            System.IO.FileStream fs;
            try { fs = System.IO.File.OpenRead("generator.config"); }
            catch
            {
                return;
            }
            System.IO.StreamReader sr = new System.IO.StreamReader(fs);
            char[] separator = new char[1];
                separator[0] = ',';
            while (!sr.EndOfStream)
            {
                string line = sr.ReadLine().Trim();
                if (line.Length <= 0) continue;

                if (line.StartsWith("//")) continue;

                try
                {
                    if (line.StartsWith("heldinput_max"))
                    {
                        heldinput_max = int.Parse(line.Substring("heldinput_max".Length).Trim());
                    }
                    else if (line.StartsWith("maxcombinations_persave"))
                    {
                        maxcombinations_persave = int.Parse(line.Substring("maxcombinations_persave".Length).Trim());
                    }
                    else if (line.StartsWith("frameinsert_do"))
                    {
                        frameinsert_do = (line.Substring("frameinsert_do".Length).Trim() == "1");
                    }
                    else if (line.StartsWith("frameinsert_min"))
                    {
                        frameinsert_min = int.Parse(line.Substring("frameinsert_min".Length).Trim());
                    }
                    else if (line.StartsWith("frameinsert_max"))
                    {
                        frameinsert_max = int.Parse(line.Substring("frameinsert_max".Length).Trim());
                    }
                    else if (line.StartsWith("frameinsert_onframes"))
                    {
                        frameinsert_onframes_original = line.Substring("frameinsert_onframes".Length).Trim();
                        string[] frames = frameinsert_onframes_original.Split(separator);
                        int[] data = new int[frames.Length];
                        for (int i = 0; i < frames.Length; i++)
                        {
                            data[i] = int.Parse(frames[i]);
                        }
                        frameinsert_onframes = new int[data.Length];
                        data.CopyTo(frameinsert_onframes, 0);
                    }
                    else if (line.StartsWith("frameinsert_framestart"))
                    {
                        frameinsert_framestart = int.Parse(line.Substring("frameinsert_framestart".Length).Trim());
                    }
                    else if (line.StartsWith("frameinsert_frameend"))
                    {
                        frameinsert_frameend = int.Parse(line.Substring("frameinsert_frameend".Length).Trim());
                    }
                    else if (line.StartsWith("frameinsert_inputs"))
                    {
                        frameinsert_inputs_original = line.Substring("frameinsert_inputs".Length).Trim();
                        if( frameinsert_inputs_original != "" )
                            frameinsert_inputs = frameinsert_inputs_original.Split(separator);
                    }
                    else if (line.StartsWith("frameinsert_combineinputs"))
                    {
                        frameinsert_combineinputs = (line.Substring("frameinsert_combineinputs".Length).Trim() != "0");
                    }
                    else if (line.StartsWith("frameinsert_pushframelist_oninsert"))
                    {
                        frameinsert_pushframelist_oninsert = (line.Substring("frameinsert_pushframelist_oninsert".Length).Trim() != "0");
                    }
                    else if (line.StartsWith("frameinsert_pushframelist_onremove"))
                    {
                        frameinsert_pushframelist_onremove = (line.Substring("frameinsert_pushframelist_onremove".Length).Trim() != "0");
                    }
                    else if (line.StartsWith("inputreplace_do"))
                    {
                        inputreplace_do = (line.Substring("inputreplace_do".Length).Trim() == "1");
                    }
                    else if (line.StartsWith("inputreplace_onframes"))
                    {
                        inputreplace_onframes_original = line.Substring("inputreplace_onframes".Length).Trim();
                        string[] frames = inputreplace_onframes_original.Split(separator);
                        int[] data = new int[frames.Length];
                        for (int i = 0; i < frames.Length; i++)
                        {
                            data[i] = int.Parse(frames[i]);
                        }
                        inputreplace_onframes = new int[data.Length];
                        data.CopyTo(inputreplace_onframes, 0);
                    }
                    else if (line.StartsWith("inputreplace_framestart"))
                    {
                        inputreplace_framestart = int.Parse(line.Substring("inputreplace_framestart".Length).Trim());
                    }
                    else if (line.StartsWith("inputreplace_frameend"))
                    {
                        inputreplace_frameend = int.Parse(line.Substring("inputreplace_frameend".Length).Trim());
                    }
                    else if (line.StartsWith("inputreplace_includeidleframes_onsearch"))
                    {
                        inputreplace_includeidleframes_onsearch = (line.Substring("inputreplace_includeidleframes_onsearch".Length).Trim() == "1");
                    }
                    else if (line.StartsWith("inputreplace_inputs"))
                    {
                        inputreplace_inputs_original = line.Substring("inputreplace_inputs".Length).Trim();
                        if (inputreplace_inputs_original != "")
                            inputreplace_inputs = inputreplace_inputs_original.Split(separator);
                    }
                    else if (line.StartsWith("inputreplace_combineinputs"))
                    {
                        inputreplace_combineinputs = (line.Substring("inputreplace_combineinputs".Length).Trim() != "0");
                    }
                    else if (line.StartsWith("inputreplace_pushframelist_oninsert"))
                    {
                        inputreplace_pushframelist_oninsert = (line.Substring("inputreplace_pushframelist_oninsert".Length).Trim() != "0");
                    }
                    else if (line.StartsWith("inputreplace_pushframelist_onremove"))
                    {
                        inputreplace_pushframelist_onremove = (line.Substring("inputreplace_pushframelist_onremove".Length).Trim() != "0");
                    }
                    else if (line.StartsWith("frameremove_do"))
                    {
                        frameremove_do = (line.Substring("frameremove_do".Length).Trim() == "1");
                    }
                    else if (line.StartsWith("frameremove_min"))
                    {
                        frameremove_min = int.Parse(line.Substring("frameremove_min".Length).Trim());
                    }
                    else if (line.StartsWith("frameremove_max"))
                    {
                        frameremove_max = int.Parse(line.Substring("frameremove_max".Length).Trim());
                    }
                    else if (line.StartsWith("frameremove_onframes"))
                    {
                        frameremove_onframes_original = line.Substring("frameremove_onframes".Length).Trim();
                        string[] frames = frameremove_onframes_original.Split(separator);
                        int[] data = new int[frames.Length];
                        for (int i = 0; i < frames.Length; i++)
                        {
                            data[i] = int.Parse(frames[i]);
                        }
                        frameremove_onframes = new int[data.Length];
                        data.CopyTo( frameremove_onframes, 0 );
                    }
                    else if (line.StartsWith("frameremove_framestart"))
                    {
                        frameremove_framestart = int.Parse(line.Substring("frameremove_framestart".Length).Trim());
                    }
                    else if (line.StartsWith("frameremove_frameend"))
                    {
                        frameremove_frameend = int.Parse(line.Substring("frameremove_frameend".Length).Trim());
                    }
                    else if (line.StartsWith("frameremove_inputs"))
                    {
                        frameremove_inputs_original = line.Substring("frameremove_inputs".Length).Trim();
                        if (frameremove_inputs_original != "")
                            frameremove_inputs = frameremove_inputs_original.Split(separator);
                    }
                    else if (line.StartsWith("frameremove_combineinputs"))
                    {
                        frameremove_combineinputs = (line.Substring("frameremove_combineinputs".Length).Trim() != "0");
                    }
                    else if (line.StartsWith("frameremove_pushframelist_oninsert"))
                    {
                        frameremove_pushframelist_oninsert = (line.Substring("frameremove_pushframelist_oninsert".Length).Trim() != "0");
                    }
                    else if (line.StartsWith("frameremove_pushframelist_onremove"))
                    {
                        frameremove_pushframelist_onremove = (line.Substring("frameremove_pushframelist_onremove".Length).Trim() != "0");
                    }
                }
                catch (SystemException)
                {
                    continue;
                }
            }
            sr.Close(); sr = null; fs.Dispose();
        }

        public bool Preprocess()
        {
            if (movie == null)
            {
                MessageBox.Show(
                    "No movie loaded.",
                    "Preprocessing",
                    MessageBoxButtons.OK, MessageBoxIcon.None, MessageBoxDefaultButton.Button1, MessageBoxOptions.ServiceNotification);
                return false;
            };

            finishrunning = false;


            //1. Calculate new movie possible length
            //  Removals or replacements ont modify length, only insertion.
            // +1 just in case
            int newmovielen = movie.Input.FrameData.Length + frameinsert_max + 1;

            baseinput = new MovieSplicer.Data.TASMovieInput[newmovielen];
            for (int i=0; i < baseinput.Length; i++)
            {
                baseinput[i] = new MovieSplicer.Data.TASMovieInput();
                for (int j = 0; j < movie.Input.ControllerCount; j++)
                {
                    baseinput[i].Controller[j] = "";
                    //ie. genesis requires two controllers at any time...
                }
            }


            bool inputsearch_insert = false;
            bool inputsearch_remove = false;
            bool inputsearch_replace = false;

            if (frameinsert_frameend < 0)
                frameinsert_frameend = movie.Input.FrameData.Length - 1;
            if (frameremove_frameend < 0)
                frameremove_frameend = movie.Input.FrameData.Length - 1;
            if (inputreplace_frameend < 0)
                inputreplace_frameend = movie.Input.FrameData.Length - 1;
            //@bad - sorry but it complicates stuff
            //you won't be able to remove 20 frames starting frame 0
            if (frameremove_framestart < frameremove_max)
                frameremove_framestart = frameremove_max;


            //IF we have the _onframes variables set, then sort them
            //2. check insert
            if (frameinsert_min < 1 && frameinsert_max < 1)
            {
                frameinsert_do = false;
            };
            if (frameinsert_do)
            {
                if (frameinsert_inputs == null)
                {
                    frameinsert_inputs = new string[0];
                };
                if (frameinsert_onframes.Length == 0)
                {
                    inputsearch_insert = true;
                }
                else
                {
                    SortIntArray(ref frameinsert_onframes);
                }

                //combine inputs if required
                if (frameinsert_combineinputs)
                {
                    GenerateAllInputCombinations(ref frameinsert_inputs);
                }
                else
                {
                    movie.CleanInputList(ref frameinsert_inputs);
                }
            }
            else
            {
                frameinsert_min = 0;
                frameinsert_max = 0;
                frameinsert_onframes = new int[0];
                frameinsert_onframes_original = "";
                frameinsert_framestart = 0;
                frameinsert_frameend = 0;
            }

            //3. check removal
            if (frameremove_min < 1 && frameremove_max < 1)
            {
                frameremove_do = false;
            };
            if (frameremove_do)
            {
                if (frameremove_inputs == null)
                {
                    frameremove_inputs = new string[0];
                };
                if (frameremove_onframes.Length == 0)
                {
                    inputsearch_remove = true;
                }
                else
                {
                    SortIntArray(ref frameremove_onframes);
                }

                //combine inputs if required
                if (frameremove_combineinputs)
                {
                    //any input
                    if (frameremove_inputs.Length == 0)
                    {
                    }
                    else
                    {
                        GenerateAllInputCombinations(ref frameremove_inputs);
                    }
                }
                else
                {
                    movie.CleanInputList(ref frameremove_inputs);
                }
            }
            else
            {
                frameremove_min = 0;
                frameremove_max = 0;
                frameremove_onframes = new int[0];
                frameremove_onframes_original = "";
                frameremove_framestart = 0;
                frameremove_frameend = 0;
            }


            //4. check replacement
            if (inputreplace_do)
            {
                if (inputreplace_inputs == null)
                {
                    inputreplace_inputs = new string[0];
                };

                if (inputreplace_onframes.Length == 0)
                {
                    inputsearch_replace = true;
                }
                else
                {
                    SortIntArray(ref inputreplace_onframes);
                }

                //combine inputs if required
                if (inputreplace_combineinputs)
                {
                    GenerateAllInputCombinations(ref inputreplace_inputs);
                }
                else
                {
                    movie.CleanInputList(ref inputreplace_inputs);
                }
            }
            else
            {
                inputreplace_onframes = new int[0];
                inputreplace_onframes_original = "";
                inputreplace_framestart = 0;
                inputreplace_frameend = 0;
            }


            //5. See possibility of a movie input "header"
            //any input before this should be copied now to base_input
            //since it won't be overwritten
            movieheader_endindex = baseinput.Length;
            if (frameinsert_do)
            {
                if (inputsearch_insert)
                {
                    if (movieheader_endindex > frameinsert_framestart)
                        movieheader_endindex = frameinsert_framestart;
                }
                else
                {
                    if (movieheader_endindex > frameinsert_onframes[0])
                        movieheader_endindex = frameinsert_onframes[0];
                }
            };
            if (frameremove_do)
            {
                if (inputsearch_remove)
                {
                    if (movieheader_endindex > frameremove_framestart)
                        movieheader_endindex = frameremove_framestart-frameremove_max;
                }
                else
                {
                    if (movieheader_endindex > frameremove_onframes[0])
                        movieheader_endindex = frameremove_onframes[0]-frameremove_max;
                }
            };
            if (inputreplace_do)
            {
                if (inputsearch_replace)
                {
                    if (movieheader_endindex > inputreplace_framestart)
                        movieheader_endindex = inputreplace_framestart;
                }
                else
                {
                    if (movieheader_endindex > inputreplace_onframes[0])
                        movieheader_endindex = inputreplace_onframes[0];
                }
            };

            for ( int i = 0; i < movieheader_endindex; i++)
            {
                if( i >= movie.Input.FrameData.Length) break;
                for (int j = 0; j < movie.Input.ControllerCount; j++ )
                {
                    baseinput[i].Controller[j] = movie.Input.FrameData[i].Controller[j];
                }
            }

            //6. Build input list
            /// Just a buffer of input that the generator
            /// must not forget to add back to the movie
            ArrayList framesWithInputArray = new ArrayList();
            FrameWithInput fwi = new FrameWithInput(heldinput_max); 
            String lastInput = "";
            int inputheldfor = 0;
            bool fwiset = false;

            //7. Perform input searches
            ArrayList insertCombinationsArray = new ArrayList();
            ArrayList removeCombinationsArray = new ArrayList();
            ArrayList replaceCombinationsArray = new ArrayList();
            
            //If there is input, add it to our list, we have to push this data
            for (int i = movieheader_endindex; i < movie.Input.FrameData.Length; i++)
            {
                String s = movie.Input.FrameData[i].Controller[0];
                if (s != lastInput)
                {
                    inputheldfor = 1;
                }
                else
                {
                    ++inputheldfor;
                    if (inputheldfor > heldinput_max && heldinput_max > 0 )
                        inputheldfor = 1;
                }
                lastInput = s;

                if (inputheldfor == 1)
                {
                    if (inputsearch_insert)
                    {
                        if (frameinsert_framestart <= i
                            && frameinsert_frameend >= i)
                        {
                            string[] sa = new string[0];
                            if (IsIncludedInInput(ref s, ref sa, true, false))
                            {
                                Combination c = new Combination();
                                c.total_values = frameinsert_max;
                                c.value = 0;
                                c.action = Combination.Actions.INSERT;
                                c.frame = i;
                                insertCombinationsArray.Add(c);
                            };
                        };
                    };
                    if (inputsearch_remove)
                    {
                        if (frameremove_framestart <= i
                            && frameremove_frameend >= i)
                        {
                            if (IsIncludedInInput(ref s, ref frameremove_inputs, true, true))
                            {
                                Combination c = new Combination();
                                c.total_values = frameremove_max;
                                c.value = 0;
                                c.action = Combination.Actions.REMOVE;
                                c.frame = i;
                                removeCombinationsArray.Add(c);
                            };
                        };
                    };
                    if (inputsearch_replace)
                    {
                        if (inputreplace_framestart <= i
                            && inputreplace_frameend >= i)
                        {
                            string[] sa = new string[0];
                            if (IsIncludedInInput(ref s, ref sa, true, inputreplace_includeidleframes_onsearch))
                            {
                                Combination c = new Combination();
                                c.total_values = inputreplace_inputs.Length;
                                c.value = 0;
                                c.action = Combination.Actions.REPLACE;
                                c.frame = i;
                                replaceCombinationsArray.Add(c);
                            };
                        };
                    };
                };

                if (s == "") continue;

                if (inputheldfor == 1 || fwi.total_input == fwi.Input.Length)
                {
                    if (fwiset) framesWithInputArray.Add(fwi);
                    fwi = new FrameWithInput(heldinput_max);
                    fwi.newframeposition = 0;
                    fwi.originalframeposition = i;
                    fwiset = true;
                };

                fwi.Input[fwi.total_input].Controller[0] = s;
                fwi.Input[fwi.total_input].Controller[1] = "";
                fwi.total_input++;
            }
            if (fwiset) framesWithInputArray.Add(fwi);
            framesWithInput = (FrameWithInput[]) framesWithInputArray.ToArray(typeof(FrameWithInput));


            if (!inputsearch_insert)
            {
                for ( int i = 0; i< frameinsert_onframes.Length ; i++ )
                {
                    Combination c = new Combination();
                    c.total_values = frameinsert_max;
                    c.value = 0;
                    c.action = Combination.Actions.INSERT;
                    c.frame = frameinsert_onframes[i];
                    insertCombinationsArray.Add(c);
                }
            };
            if (!inputsearch_remove)
            {
                for (int i = 0; i < frameremove_onframes.Length; i++)
                {
                    Combination c = new Combination();
                    c.total_values = frameremove_max;
                    c.value = 0;
                    c.action = Combination.Actions.REMOVE;
                    c.frame = frameremove_onframes[i];
                    removeCombinationsArray.Add(c);
                };
            };
            if (!inputsearch_replace)
            {
                for (int i = 0; i < inputreplace_onframes.Length; i++)
                {
                    Combination c = new Combination();
                    c.total_values = inputreplace_inputs.Length;
                    c.value = 0;
                    c.action = Combination.Actions.REPLACE;
                    c.frame = inputreplace_onframes[i];
                    replaceCombinationsArray.Add(c);
                };
            };

            insertCombinations = (Combination[])insertCombinationsArray.ToArray(typeof(Combination));
            removeCombinations = (Combination[])removeCombinationsArray.ToArray(typeof(Combination));
            replaceCombinations = (Combination[])replaceCombinationsArray.ToArray(typeof(Combination));

            if (!frameinsert_do && !frameremove_do && !inputreplace_do)
            {
                MessageBox.Show(
                        "Open generator.config and edit the values, you do not want to do any generation according to the file. In any case use the real tas-movie-editor if you want to edit movies as usual, this program only generates random movies.",
                        "Couldn't generate movies",
                        MessageBoxButtons.OK, MessageBoxIcon.None, MessageBoxDefaultButton.Button1, MessageBoxOptions.ServiceNotification);

                return false;
            };

            return true;
        }

        public void Generate( MovieSplicer.UI.frmMain parent )
        {
            int movielen = movie.Input.FrameData.Length;

            if (frameinsert_current < 0)
            {
                frameinsert_current = frameinsert_min;
            }
            if (frameremove_current < 0)
            {
                frameremove_current = frameremove_max;
            }

            while(frameinsert_current <= frameinsert_max)
            {
                while(frameremove_current >= frameremove_min)
                {
                    SaveMoviesForGeneration();
                    MessageBox.Show("Total Movies Generated so far: " + lastmoviesaved_index.ToString(), "Generation step done", MessageBoxButtons.OK, MessageBoxIcon.None, MessageBoxDefaultButton.Button1, MessageBoxOptions.ServiceNotification);
                    --frameremove_current;
                    if (finishrunning) break;
                }
                frameremove_current = frameremove_max;
                ++frameinsert_current;
                if (finishrunning) break;
            }
            frameinsert_current = 0;

            //movie.Input.FrameData[0].Controller[0] = lastmoviesaved_index.ToString();
            MessageBox.Show( "Total Movies Generated so far: " + lastmoviesaved_index.ToString(), "Generation step done", MessageBoxButtons.OK, MessageBoxIcon.None, MessageBoxDefaultButton.Button1, MessageBoxOptions.ServiceNotification);
            if (finishrunning)
                MessageBox.Show("When there are more than "+maxcombinations_persave.ToString()+" movies generated the generation is stopped and saved, if you want to restart just delete the file generation.save, if you want to continue generating, use Save again. You may close the program anytime you wish or even delete the files already generated (after you finish using them of course).", "The generation is not complete!", MessageBoxButtons.OK, MessageBoxIcon.None, MessageBoxDefaultButton.Button1, MessageBoxOptions.ServiceNotification);
            else
            {
                System.IO.File.Delete("generator.save");
                MessageBox.Show("generation.save (if any) was deleted!", "Generation finished!", MessageBoxButtons.OK, MessageBoxIcon.None, MessageBoxDefaultButton.Button1, MessageBoxOptions.ServiceNotification);
            }
        }

        public void SaveMoviesForGeneration()
        {
            //1. prepare combinations
            if (restored_fromsave)
            {
                char[] separator = new char[1];
                separator[0] = ',';
                
                string []vals;
                vals = insert_combinations_restore.Split(separator);
                for (int i = 0; i < insertCombinations.Length; i++)
                {
                    insertCombinations[i].value = int.Parse(vals[i]);
                    insertCombinations[i].total_values = frameinsert_current;
                }
                vals = remove_combinations_restore.Split(separator);
                for (int i = 0; i < removeCombinations.Length; i++)
                {
                    removeCombinations[i].value = int.Parse(vals[i]);
                    removeCombinations[i].total_values = frameremove_current;
                }
                vals = replace_combinations_restore.Split(separator);
                for (int i = 0; i < replaceCombinations.Length; i++)
                {
                    replaceCombinations[i].value = int.Parse(vals[i]);
                }

                //Initialization of sub combinations of insert
                insertSubCombinations = new Combination[frameinsert_current];
                vals = insert_subcombinations_restore.Split(separator);
                for (int i = 0; i < insertSubCombinations.Length; i++)
                {
                    insertSubCombinations[i].value = int.Parse(vals[i]);
                    insertSubCombinations[i].total_values = frameinsert_inputs.Length;
                    insertSubCombinations[i].action = Combination.Actions.INSERTSUBCOMBINATION;
                }
            }
            else
            {
                for (int i = 0; i < insertCombinations.Length; i++)
                {
                    insertCombinations[i].value = 0;
                    insertCombinations[i].total_values = frameinsert_current;
                }
                for (int i = 0; i < removeCombinations.Length; i++)
                {
                    removeCombinations[i].value = 0;
                    removeCombinations[i].total_values = frameremove_current;
                }
                for (int i = 0; i < replaceCombinations.Length; i++)
                {
                    replaceCombinations[i].value = 0;
                }

                //Initialization of sub combinations of insert
                insertSubCombinations = new Combination[frameinsert_current];
                for (int i = 0; i < insertSubCombinations.Length; i++)
                {
                    insertSubCombinations[i].value = 0;
                    insertSubCombinations[i].total_values = frameinsert_inputs.Length;
                    insertSubCombinations[i].action = Combination.Actions.INSERTSUBCOMBINATION;
                }


                //2. combine
                //first do all removals
                //then do all insertions
                //then do all replacements
                //avoid crash
                if (frameremove_do && frameremove_current > 0)
                {
                    removeCombinations[removeCombinations.Length - 1].value = frameremove_current; //start in first iteration
                    frameremove_iteration = removeCombinations.Length - 1 - 1; //this will put the iteration ready for entering and doing the next iteration
                    frameremove_totalframesremovedsofar = frameremove_current; //starts ready for second combination
                }
                else
                {
                    frameremove_iteration = -1;
                }
            
            //    frameremove_iteration = -1;
              //  frameremove_totalframesremovedsofar = 0;

                //avoid crash
                if (frameinsert_do && frameinsert_current > 0 )
                {
                    insertCombinations[insertCombinations.Length - 1].value = frameinsert_current; //start in first iteration
                    frameinsert_iteration = insertCombinations.Length - 1 - 1; //this will put the iteration ready for entering and doing the next iteration
                    frameinsert_totalframesinsertedsofar = frameinsert_current; //starts ready for second combination
                }
                else
                {
                    frameinsert_iteration = -1;
                }
            }

            while(true)
            {
                if (restored_fromsave)
                {
                    restored_fromsave = false;
                }
                else
                {
                    ++lastmoviesaved_index;
                    dosave();
                    ++numbermoviessavedthissession;
                    if (numbermoviessavedthissession > maxcombinations_persave)
                    {
                        SaveSession();
                        finishrunning = true;
                        return;
                    };
                };
                
                
                //1. Update combinations for next save
                //update remove combinations
                //in case of overflow value is left broken
                //so next call sets it to first value
                if (frameremove_iteration == -1)
                {
                    //next turn we will start saving the results of current turn
                    addonemoreinsertsub = true; //and add one to next combination
                }
                MoveFrameRemoveGenerationForwardOneStep();
                
                if (addonemoreinsertsub)
                {
                    addonemoreinsertsub = false;
                    if ( MoveFrameInsertSubGenerationForwardOneStep() )
                    {
                        if (frameinsert_iteration == -1)
                        {
                            addonemorereplace = true;
                        }

                        MoveFrameInsertGenerationForwardOneStep();

                        if (addonemorereplace)
                        {
                            addonemorereplace = false;
                            if( MoveFrameReplaceGenerationForwardOneStep() )
                            {
                                break;
                            };
                        };
                    };
                };
            }
        }

        private void MoveFrameRemoveGenerationForwardOneStep()
        {
            //dont do this, do insert or replace
            if (!frameremove_do || frameremove_current == 0 )
            {
                frameremove_iteration = -1;
                return;
            };

            bool addonemore = true;
            
            //reset last combination step
            //first time this is zero
            frameremove_totalframesremovedsofar -= removeCombinations[frameremove_iteration+1].value;
            removeCombinations[frameremove_iteration+1].value = 0;

            if (frameremove_iteration < 0)
            {
                frameremove_iteration = removeCombinations.Length - 1;
            }

            for (; frameremove_iteration < removeCombinations.Length; frameremove_iteration++)
            {
                //1. Update combinations for next save
                //update remove combinations
                int k = (frameremove_iteration != removeCombinations.Length - 1 ? (addonemore ? 1 : 0) : frameremove_current - frameremove_totalframesremovedsofar);
                removeCombinations[frameremove_iteration].value += k;
                frameremove_totalframesremovedsofar += k;
                addonemore = false;

                if (frameremove_totalframesremovedsofar != frameremove_current)
                {
                    continue;
                }
                else
                {
                    //new combination!
                    --frameremove_iteration; //do again the one behind
                    //if it gets set to -1 then it will cause an overflow
                    //to the next combination mode!
                    return;
                }
             }

            //safety
            frameremove_iteration = -1;
         }

        private void MoveFrameInsertGenerationForwardOneStep()
        {
            //dont do this, do removes or replaces
            if (!frameinsert_do || frameinsert_current == 0 )
            {
                frameinsert_iteration = -1;
                return;
            };
            
            bool addonemore = true;

            //reset last combination step
            //first time this is zero
            frameinsert_totalframesinsertedsofar -= insertCombinations[frameinsert_iteration+1].value;
            insertCombinations[frameinsert_iteration+1].value = 0;

            if (frameinsert_iteration < 0)
            {
                frameinsert_iteration = insertCombinations.Length - 1;
            }

            for (; frameinsert_iteration < insertCombinations.Length; frameinsert_iteration++)
            {
                //1. Update combinations for next save
                //update insert combinations
                int k = (frameinsert_iteration != insertCombinations.Length - 1 ? (addonemore ? 1 : 0) : frameinsert_current - frameinsert_totalframesinsertedsofar);
                insertCombinations[frameinsert_iteration].value += k;
                frameinsert_totalframesinsertedsofar += k;
                addonemore = false;

                if (frameinsert_totalframesinsertedsofar != frameinsert_current)
                {
                    continue;
                }
                else
                {
                    //new combination!
                    --frameinsert_iteration; //do again the one behind
                    return;
                }
            }

            //safety
            frameinsert_iteration = -1;
        }

        public bool MoveFrameInsertSubGenerationForwardOneStep()
        {
            //dont do this, do removes or replaces
            if (!frameinsert_do)
            {
                return true;
            };

            //update insert combinations
            for (int j = 0; j < frameinsert_current; j++)
            {
                // 0 is a valid value
                //[0,total-1]
                ++insertSubCombinations[j].value;
                if (insertSubCombinations[j].value > insertSubCombinations[j].total_values-1)
                {
                    insertSubCombinations[j].value = 0;
                    if (j == frameinsert_current - 1)
                    {
                        //we finished the combinations of this mode!
                        //be sure to increase the next combination mode!
                        return true;
                    }
                    continue;
                }
                
                //new combination!
                return false;
            }

            //safety
            return true;
        }

        public bool MoveFrameReplaceGenerationForwardOneStep()
        {
            //dont do this, do removes or inserts
            if (!inputreplace_do)
            {
                return true;
            };

            //update replace combinations
            for (int j = 0; j < replaceCombinations.Length; j++)
            {
                // 0 is a valid value
                //[0,total-1]
                ++replaceCombinations[j].value;
                if (replaceCombinations[j].value > replaceCombinations[j].total_values-1)
                {
                    replaceCombinations[j].value = 0;
                    if (j == replaceCombinations.Length - 1)
                    {
                        return true;
                    }
                    continue;
                }

                //new combination!
                return false;
            }

            //safety
            return true;
        }

        private void dosave()
        {
            int insertoffset = 0;
            int removeoffset = 0;

            int frame = 0;
            int index_input = 0;
            int index_heldinput = 0;
            int index_insert = 0;
            int index_insertframe = 0;
            int index_insertsubcomb = 0;
            int index_remove = 0;
            int index_replace = 0;
            
            bool end = false;
            while (true)
            {
                end = true;

                //1. first remove
                if( index_remove < removeCombinations.Length )
                {
                    if (removeCombinations[index_remove].frame <= frame + (frameremove_pushframelist_oninsert ? 0 : insertoffset) + (frameremove_pushframelist_onremove ? 0 : removeoffset))
                    {
                        removeoffset -= removeCombinations[index_remove].value;
                        ++index_remove;
                    };
                    end = false;
                };

                //2. then insert
                if (index_insert < insertCombinations.Length)
                {
                    if (insertCombinations[index_insert].frame <= frame + (frameinsert_pushframelist_oninsert ? 0 : insertoffset) + (frameinsert_pushframelist_onremove ? 0 : removeoffset))
                    {
                        //insert frames
                        if (index_insertframe < insertCombinations[index_insert].value)
                        {
                            baseinput[frame + insertoffset + removeoffset].Controller[0] = frameinsert_inputs[insertSubCombinations[index_insertsubcomb].value];
                            ++insertoffset;
                            ++index_insertsubcomb;
                            ++index_insertframe;
                            continue;
                        }
                        else
                        {
                            ++index_insert;
                            index_insertframe = 0;
                        }
                    }
                    end = false;
                };

                //3. then put the normal input
                if (index_input < framesWithInput.Length)
                {
                    if( framesWithInput[index_input].originalframeposition+index_heldinput <= frame )
                    {
                        baseinput[frame+insertoffset+removeoffset].Controller[0] = framesWithInput[index_input].Input[index_heldinput].Controller[0];
                        ++index_heldinput;
                        if( index_heldinput >= framesWithInput[index_input].total_input )
                        {
                            index_heldinput = 0;
                            ++index_input;
                        };
                    }
                    end = false;
                };

                //4. then replace input
                if (index_replace < replaceCombinations.Length)
                {
                    if (replaceCombinations[index_replace].frame <= frame + (inputreplace_pushframelist_oninsert ? 0 : insertoffset) + (inputreplace_pushframelist_onremove ? 0 : removeoffset))
                    {
                        baseinput[frame+insertoffset+removeoffset].Controller[0] = inputreplace_inputs[replaceCombinations[index_replace].value];
                        ++index_replace;
                    }
                    end = false;
                };

                ++frame;
                if (end) break;
                if (frame < movie.Input.FrameData.Length) continue;
                break;
            }
            
            movie.Save(lastmoviesaved_index.ToString() + movie.Filename.Substring(movie.Filename.IndexOf(".")), ref baseinput);

            //clear garbage
            for (int i = movieheader_endindex; i < movie.Input.FrameData.Length; i++)
            {
                baseinput[i].Controller[0] = "";
            }
        }


        public void SaveSession()
        {
            bool first = true;
            long combs = 1;
            try
            {
                System.IO.TextWriter tw = new System.IO.StreamWriter("generator.save");

                tw.WriteLine( "//this file is created when all combinations" );
                tw.WriteLine( "//weren't created because of the size of the query" );
                tw.WriteLine();
                tw.WriteLine( "//delete this file if you want to start fresh" );
                tw.WriteLine();
                tw.WriteLine("//");
                tw.Write( "heldinput_max " );
                tw.WriteLine( heldinput_max );
                tw.WriteLine("//");
                tw.Write( "maxcombinations_persave " );
                tw.WriteLine( maxcombinations_persave );
                tw.WriteLine("//");
                tw.Write( "frameinsert_do " );
                tw.WriteLine( frameinsert_do ? "1" : "0" );
                tw.WriteLine("//");
                tw.Write( "frameinsert_min " );
                tw.WriteLine( frameinsert_min );
                tw.WriteLine("//");
                tw.Write( "frameinsert_max " );
                tw.WriteLine( frameinsert_max );
                tw.WriteLine("//");
                tw.Write( "frameinsert_onframes " );
                tw.WriteLine( frameinsert_onframes_original );
                tw.WriteLine("//");
                tw.Write( "frameinsert_framestart " );
                tw.WriteLine( frameinsert_framestart );
                tw.WriteLine("//");
                tw.Write( "frameinsert_frameend " );
                tw.WriteLine( frameinsert_frameend );
                tw.WriteLine("//");
                tw.Write( "frameinsert_inputs " );
                tw.WriteLine( frameinsert_inputs_original );
                tw.WriteLine("//");
                tw.Write( "frameinsert_combineinputs " );
                tw.WriteLine( frameinsert_combineinputs ? "1" : "0" );
                tw.WriteLine("//");
                tw.Write( "frameinsert_pushframelist_oninsert " );
                tw.WriteLine( frameinsert_pushframelist_oninsert ? "1" : "0" );
                tw.WriteLine("//");
                tw.Write( "frameinsert_pushframelist_onremove " );
                tw.WriteLine(frameinsert_pushframelist_onremove ? "1" : "0");
                tw.WriteLine("//");
                tw.Write( "inputreplace_do " );
                tw.WriteLine(inputreplace_do ? "1" : "0");
                tw.WriteLine("//");
                tw.Write( "inputreplace_onframes " );
                tw.WriteLine( inputreplace_onframes_original );
                tw.WriteLine("//");
                tw.Write( "inputreplace_framestart " );
                tw.WriteLine( inputreplace_framestart );
                tw.WriteLine("//");
                tw.Write( "inputreplace_frameend " );
                tw.WriteLine( inputreplace_frameend );
                tw.WriteLine("//");
                tw.Write( "inputreplace_includeidleframes_onsearch " );
                tw.WriteLine(inputreplace_includeidleframes_onsearch ? "1" : "0");
                tw.WriteLine("//");
                tw.Write( "inputreplace_inputs " );
                tw.WriteLine( inputreplace_inputs_original );
                tw.WriteLine("//");
                tw.Write( "inputreplace_combineinputs " );
                tw.WriteLine(inputreplace_combineinputs ? "1" : "0");
                tw.WriteLine("//");
                tw.Write( "inputreplace_pushframelist_oninsert " );
                tw.WriteLine(inputreplace_pushframelist_oninsert ? "1" : "0");
                tw.WriteLine("//");
                tw.Write( "inputreplace_pushframelist_onremove " );
                tw.WriteLine(inputreplace_pushframelist_onremove ? "1" : "0");
                tw.WriteLine("//");
                tw.Write( "frameremove_do " );
                tw.WriteLine(frameremove_do ? "1" : "0");
                tw.WriteLine("//");
                tw.Write( "frameremove_min " );
                tw.WriteLine( frameremove_min );
                tw.WriteLine("//");
                tw.Write( "frameremove_max " );
                tw.WriteLine( frameremove_max );
                tw.WriteLine("//");
                tw.Write( "frameremove_onframes " );
                tw.WriteLine( frameremove_onframes_original );
                tw.WriteLine("//");
                tw.Write( "frameremove_framestart " );
                tw.WriteLine( frameremove_framestart );
                tw.WriteLine("//");
                tw.Write( "frameremove_frameend " );
                tw.WriteLine( frameremove_frameend );
                tw.WriteLine("//");
                tw.Write( "frameremove_inputs " );
                tw.WriteLine( frameremove_inputs_original );
                tw.WriteLine("//");
                tw.Write( "frameremove_combineinputs " );
                tw.WriteLine(frameremove_combineinputs ? "1" : "0");
                tw.WriteLine("//");
                tw.Write( "frameremove_pushframelist_oninsert " );
                tw.WriteLine(frameremove_pushframelist_oninsert ? "1" : "0");
                tw.WriteLine("//");
                tw.Write( "frameremove_pushframelist_onremove " );
                tw.WriteLine(frameremove_pushframelist_onremove ? "1" : "0");
                tw.WriteLine("//");
                tw.Write( "frameinsert_current " );
                tw.WriteLine( frameinsert_current );
                tw.WriteLine("//");
                tw.Write( "frameremove_current " );
                tw.WriteLine( frameremove_current );
                tw.WriteLine("//");
                tw.Write( "frameinsert_iteration " );
                tw.WriteLine( frameinsert_iteration );
                tw.WriteLine("//");
                tw.Write( "frameremove_iteration " );
                tw.WriteLine( frameremove_iteration );
                tw.WriteLine("//");
                tw.Write( "frameinsert_totalframesinsertedsofar " );
                tw.WriteLine( frameinsert_totalframesinsertedsofar );
                tw.WriteLine("//");
                tw.Write( "frameremove_totalframesremovedsofar " );
                tw.WriteLine( frameremove_totalframesremovedsofar );
/*  not required
                tw.WriteLine("//");
                tw.Write( "addonemore " );
                tw.WriteLine(addonemore ? "1" : "0");
                tw.WriteLine("//");
                tw.Write( "movieheader_endindex " );
                tw.WriteLine( movieheader_endindex );
 */
                tw.WriteLine("//");
                tw.Write( "insert_combinations " );
                first = true;
                for( int i = 0; i < insertCombinations.Length; i++ )
                {
                    if (!first) tw.Write(",");
                    first = false;
                    tw.Write( insertCombinations[i].value.ToString() );
                }
                tw.WriteLine();
                tw.WriteLine("//This one is just info for you: (Not calculated for now, its a bit complex)");
                tw.Write("total_insert_combinations_to_generate ");
                tw.WriteLine(combs);
                tw.WriteLine("//");
                tw.Write( "remove_combinations " );
                first = true;
                for( int i = 0; i < removeCombinations.Length; i++ )
                {
                    if (!first) tw.Write(",");
                    first = false;
                    tw.Write( removeCombinations[i].value.ToString() );
                }
                tw.WriteLine();
                tw.WriteLine("//This one is just info for you: (Not calculated for now, its a bit complex)");
                tw.Write("total_remove_combinations_to_generate ");
                tw.WriteLine(combs);
                tw.WriteLine("//");
                tw.Write( "replace_combinations " );
                first = true;
                for( int i = 0; i < replaceCombinations.Length; i++ )
                {
                    combs *= replaceCombinations[i].total_values;
                    if (!first) tw.Write(",");
                    first = false;
                    tw.Write( replaceCombinations[i].value.ToString() );
                }
                tw.WriteLine();
                tw.WriteLine("//This one is just info for you:");
                tw.Write("total_replace_combinations_to_generate ");
                tw.WriteLine( combs );
                tw.WriteLine("//");
                tw.Write( "insert_subcombinations " );
                first = true;
                for( int i = 0; i < insertSubCombinations.Length; i++ )
                {
                    if (!first) tw.Write(",");
                    first = false;
                    tw.Write( insertSubCombinations[i].value.ToString() );
                }
                tw.WriteLine();
                tw.WriteLine("//");
                tw.Write( "lastmoviesaved_index " );
                tw.WriteLine( lastmoviesaved_index );
                tw.Close(); tw = null;
            }
            catch (System.IO.FileNotFoundException fnfe)
            {
                fnfe.GetBaseException();
            }
        }

        public bool RestoreSavedSession()
        {
            System.IO.FileStream fs;
            System.IO.StreamReader sr;
            try
            {
                fs = System.IO.File.OpenRead("generator.save");
                sr = new System.IO.StreamReader(fs);
                MessageBox.Show("generator.save was detected, the generation will continue from this point! If you do not wish this, you will have to delete generator.save.", "Generation restore!", MessageBoxButtons.OK, MessageBoxIcon.None, MessageBoxDefaultButton.Button1, MessageBoxOptions.ServiceNotification);
            }
            catch (System.IO.FileNotFoundException)
            {
                //If there is no file then this is not an actual error
                return false;
            }
            while (!sr.EndOfStream)
            {
                /*try
                {*/
                    string line = sr.ReadLine().Trim();
                    if (line.Length <= 0) continue;

                    if (line.StartsWith("//")) continue;

                    if (line.StartsWith("heldinput_max"))
                    {
                        heldinput_max = int.Parse(line.Substring("heldinput_max".Length).Trim());
                    }
                    else if (line.StartsWith("maxcombinations_persave"))
                    {
                        maxcombinations_persave = int.Parse(line.Substring("maxcombinations_persave".Length).Trim());
                    }
                    else if (line.StartsWith("frameinsert_do"))
                    {
                        frameinsert_do = (int.Parse(line.Substring("frameinsert_do".Length).Trim()) > 0);
                    }
                    else if (line.StartsWith("frameinsert_min"))
                    {
                        frameinsert_min = int.Parse(line.Substring("frameinsert_min".Length).Trim());
                    }
                    else if (line.StartsWith("frameinsert_max"))
                    {
                        frameinsert_max = int.Parse(line.Substring("frameinsert_max".Length).Trim());
                    }
                    else if (line.StartsWith("frameinsert_onframes"))
                    {
                        line = line.Substring("frameinsert_onframes".Length).Trim();
                        frameinsert_onframes_original = line;
                        if (line == "") continue;

                        char[] separator = new char[1];
                        separator[0] = ',';

                        string[] vals = line.Split(separator);
                        if (vals == null) return false;
                        frameinsert_onframes = new int[vals.Length];
                        for (int i = 0; i < vals.Length; i++)
                        {
                            int val = int.Parse(vals[i]);
                            frameinsert_onframes[i] = val;
                        }
                    }
                    else if (line.StartsWith("frameinsert_framestart"))
                    {
                        frameinsert_framestart = int.Parse(line.Substring("frameinsert_framestart".Length).Trim());
                    }
                    else if (line.StartsWith("frameinsert_frameend"))
                    {
                        frameinsert_frameend = int.Parse(line.Substring("frameinsert_frameend".Length).Trim());
                    }
                    else if (line.StartsWith("frameinsert_inputs"))
                    {
                        line = line.Substring("frameinsert_inputs".Length).Trim();
                        frameinsert_inputs_original = line;
                        if (line == "") continue;

                        char[] separator = new char[1];
                        separator[0] = ',';
                        string[] vals = line.Split(separator);
                        if (vals == null) return false;
                        frameinsert_inputs = new string[vals.Length];
                        for (int i = 0; i < vals.Length; i++)
                        {
                            frameinsert_inputs[i] = vals[i];
                        }
                    }
                    else if (line.StartsWith("frameinsert_combineinputs"))
                    {
                        frameinsert_combineinputs = (int.Parse(line.Substring("frameinsert_combineinputs".Length).Trim()) > 0);
                    }
                    else if (line.StartsWith("frameinsert_pushframelist_oninsert"))
                    {
                        frameinsert_pushframelist_oninsert = (int.Parse(line.Substring("frameinsert_pushframelist_oninsert".Length).Trim()) > 0);
                    }
                    else if (line.StartsWith("frameinsert_pushframelist_onremove"))
                    {
                        frameinsert_pushframelist_onremove = (int.Parse(line.Substring("frameinsert_pushframelist_onremove".Length).Trim()) > 0);
                    }
                    else if (line.StartsWith("inputreplace_do"))
                    {
                        inputreplace_do = (int.Parse(line.Substring("inputreplace_do".Length).Trim()) > 0);
                    }
                    else if (line.StartsWith("inputreplace_onframes"))
                    {
                        line = line.Substring("inputreplace_onframes".Length).Trim();
                        inputreplace_onframes_original = line;
                        if (line == "") continue;

                        char[] separator = new char[1];
                        separator[0] = ',';
                        string[] vals = line.Split(separator);
                        if (vals == null) return false;
                        inputreplace_onframes = new int[vals.Length];
                        for (int i = 0; i < vals.Length; i++)
                        {
                            int val = int.Parse(vals[i]);
                            inputreplace_onframes[i] = val;
                        }
                    }
                    else if (line.StartsWith("inputreplace_framestart"))
                    {
                        inputreplace_framestart = int.Parse(line.Substring("inputreplace_framestart".Length).Trim());
                    }
                    else if (line.StartsWith("inputreplace_frameend"))
                    {
                        inputreplace_frameend = int.Parse(line.Substring("inputreplace_frameend".Length).Trim());
                    }
                    else if (line.StartsWith("inputreplace_includeidleframes_onsearch"))
                    {
                        inputreplace_includeidleframes_onsearch = (int.Parse(line.Substring("inputreplace_includeidleframes_onsearch".Length).Trim()) > 0);
                    }
                    else if (line.StartsWith("inputreplace_inputs"))
                    {
                        line = line.Substring("inputreplace_inputs".Length).Trim();
                        inputreplace_inputs_original = line;
                        if (line == "") continue;

                        char[] separator = new char[1];
                        separator[0] = ',';
                        string[] vals = line.Split(separator);
                        if (vals == null) return false;
                        inputreplace_inputs = new string[vals.Length];
                        for (int i = 0; i < vals.Length; i++)
                        {
                            inputreplace_inputs[i] = vals[i];
                        }
                    }
                    else if (line.StartsWith("inputreplace_combineinputs"))
                    {
                        inputreplace_combineinputs = (int.Parse(line.Substring("inputreplace_combineinputs".Length).Trim()) > 0);
                    }
                    else if (line.StartsWith("inputreplace_pushframelist_oninsert"))
                    {
                        inputreplace_pushframelist_oninsert = (int.Parse(line.Substring("inputreplace_pushframelist_oninsert".Length).Trim()) > 0);
                    }
                    else if (line.StartsWith("inputreplace_pushframelist_onremove"))
                    {
                        inputreplace_pushframelist_onremove = (int.Parse(line.Substring("inputreplace_pushframelist_onremove".Length).Trim()) > 0);
                    }
                    else if (line.StartsWith("frameremove_do"))
                    {
                        frameremove_do = (int.Parse(line.Substring("frameremove_do".Length).Trim()) > 0);
                    }
                    else if (line.StartsWith("frameremove_min"))
                    {
                        frameremove_min = int.Parse(line.Substring("frameremove_min".Length).Trim());
                    }
                    else if (line.StartsWith("frameremove_max"))
                    {
                        frameremove_max = int.Parse(line.Substring("frameremove_max".Length).Trim());
                    }
                    else if (line.StartsWith("frameremove_onframes"))
                    {
                        line = line.Substring("frameremove_onframes".Length).Trim();
                        frameremove_onframes_original = line;
                        if (line == "") continue;

                        char[] separator = new char[1];
                        separator[0] = ',';
                        string[] vals = line.Split(separator);
                        if (vals == null) return false;
                        frameremove_onframes = new int[vals.Length];
                        for (int i = 0; i < vals.Length; i++)
                        {
                            int val = int.Parse(vals[i]);
                            frameremove_onframes[i] = val;
                        }
                    }
                    else if (line.StartsWith("frameremove_framestart"))
                    {
                        frameremove_framestart = int.Parse(line.Substring("frameremove_framestart".Length).Trim());
                    }
                    else if (line.StartsWith("frameremove_frameend"))
                    {
                        frameremove_frameend = int.Parse(line.Substring("frameremove_frameend".Length).Trim());
                    }
                    else if (line.StartsWith("frameremove_inputs"))
                    {
                        line = line.Substring("frameremove_inputs".Length).Trim();
                        frameremove_inputs_original = line;
                        if (line == "") continue;

                        char[] separator = new char[1];
                        separator[0] = ',';
                        string[] vals = line.Split(separator);
                        if (vals == null) return false;
                        frameremove_inputs = new string[vals.Length];
                        for (int i = 0; i < vals.Length; i++)
                        {
                            frameremove_inputs[i] = vals[i];
                        }
                    }
                    else if (line.StartsWith("frameremove_combineinputs"))
                    {
                        frameremove_combineinputs = (int.Parse(line.Substring("frameremove_combineinputs".Length).Trim()) > 0);
                    }
                    else if (line.StartsWith("frameremove_pushframelist_oninsert"))
                    {
                        frameremove_pushframelist_oninsert = (int.Parse(line.Substring("frameremove_pushframelist_oninsert".Length).Trim()) > 0);
                    }
                    else if (line.StartsWith("frameremove_pushframelist_onremove"))
                    {
                        frameremove_pushframelist_onremove = (int.Parse(line.Substring("frameremove_pushframelist_onremove".Length).Trim()) > 0);
                    }
                    else if (line.StartsWith("frameinsert_current"))
                    {
                        frameinsert_current = int.Parse(line.Substring("frameinsert_current".Length).Trim());
                    }
                    else if (line.StartsWith("frameremove_current"))
                    {
                        frameremove_current = int.Parse(line.Substring("frameremove_current".Length).Trim());
                    }
                    else if (line.StartsWith("frameinsert_iteration"))
                    {
                        frameinsert_iteration = int.Parse(line.Substring("frameinsert_iteration".Length).Trim());
                    }
                    else if (line.StartsWith("frameremove_iteration"))
                    {
                        frameremove_iteration = int.Parse(line.Substring("frameremove_iteration".Length).Trim());
                    }
                    else if (line.StartsWith("frameinsert_totalframesinsertedsofar"))
                    {
                        frameinsert_totalframesinsertedsofar = int.Parse(line.Substring("frameinsert_totalframesinsertedsofar".Length).Trim());
                    }
                    else if (line.StartsWith("frameremove_totalframesremovedsofar"))
                    {
                        frameremove_totalframesremovedsofar = int.Parse(line.Substring("frameremove_totalframesremovedsofar".Length).Trim());
                    }
                    /* not required
                     *      private bool addonemore;
                            private int movieheader_endindex;
                     * */
                    else if (line.StartsWith("insert_combinations"))
                    {
                        insert_combinations_restore = line.Substring("insert_combinations".Length).Trim();
                    }
                    else if (line.StartsWith("remove_combinations"))
                    {
                        remove_combinations_restore = line.Substring("remove_combinations".Length).Trim();
                    }
                    else if (line.StartsWith("replace_combinations"))
                    {
                        replace_combinations_restore = line.Substring("replace_combinations".Length).Trim();
                    }
                    else if (line.StartsWith("insert_subcombinations"))
                    {
                        insert_subcombinations_restore = line.Substring("insert_subcombinations".Length).Trim();
                    }
                    else if (line.StartsWith("lastmoviesaved_index"))
                    {
                        lastmoviesaved_index = int.Parse(line.Substring("lastmoviesaved_index".Length).Trim());
                    }
               /* }
                catch (System.SystemException se)
                {
                    MessageBox.Show(
                            "Some error while loading: " + fnfe.GetBaseException(),
                            "Restore Saved Session",
                            MessageBoxButtons.OK, MessageBoxIcon.None, MessageBoxDefaultButton.Button1, MessageBoxOptions.ServiceNotification);
                    
                }*/
            }
            sr.Close(); sr = null; fs.Dispose();

            restored_fromsave = true;
            return true;
        }

        public void GenerateAllInputCombinations(ref string[] inputlist )
        {
            bool includeidle = false;

            if( inputlist == null )
            {
                includeidle = true;
                inputlist = movie.GetUsableInputs();
            }
            else if( inputlist.Length == 0 )
            {
                includeidle = true;
                inputlist = movie.GetUsableInputs();
            };

            ArrayList inputCombinations = new ArrayList();
            ArrayList inputsArray = new ArrayList();
            foreach( string input in inputlist )
            {
                if (input == "i")
                {
                    includeidle = true;
                    continue;
                }
                inputsArray.Add(input);
            }

            if (includeidle)
            {
                inputCombinations.Add("");
            };
            string[] inputs = (string[])inputsArray.ToArray(typeof(string));
            int [] combs = new int[inputs.Length];
            for (int i = 0; i < combs.Length; i++)
            {
                combs[i] = 0;
            }

            //if combs = 0 then dont use this input
            //if combs = 1 then use this input
            //so we got 10 values on base 2,
            //and that generates all combinations
            //all 0 = idle, but we already added that one
            //so try something else
            for (int j = 0; j < combs.Length; j++)
            {
                combs[j]++;
                if (combs[j] > 1)
                {
                    combs[j] = 0;
                    continue;
                }
                
                string s = "";
                for (int i = 0; i < combs.Length; i++)
                {
                    s += (combs[i] == 0 ? "" : inputs[i]);
                }
                inputCombinations.Add(s);

                j = -1;//so it sets to zero next time
            }

            inputlist = (string[])inputCombinations.ToArray(typeof(string));
        }

        public bool IsIncludedInInput(ref string s, ref string[] inputs, bool include_inputs, bool include_idles )
        {
            bool dontsearch = false;
            if (inputs == null)
            {
                dontsearch = true;
            }
            else if( inputs.Length == 0 )
            {
                dontsearch = true;
            }
            if (dontsearch)
            {
                if (s == "")
                {
                    if (include_idles) return true; 
                }
                else
                {
                    if (include_inputs) return true;
                }
                return false;
            }

            for (int i = 0; i < inputs.Length; i++)
            {
                if( s == inputs[i] ) return true;
            }
            return false;
        }

        public void SortIntArray(ref int[] intarray)
        {
            int[] temp = new int[intarray.Length];
            intarray.CopyTo(temp, 0);
            int minval = int.MaxValue;
            int best = -1;
            for (int i = 0; i < temp.Length; i++)
            {
                for (int j = 0; j < intarray.Length; j++)
                {
                    if (minval > intarray[j] && intarray[j] > -1)
                    {
                        minval = intarray[j];
                        best = j;
                    };
                }
                if (best == -1) break;
                intarray[best] = -1;
                temp[i] = minval;
                minval = int.MaxValue;
                best = -1;
            }
            temp.CopyTo(intarray, 0);
        }

    }
}