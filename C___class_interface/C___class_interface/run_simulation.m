function run_example()
    % Check if the mex exists
    dir = fileparts(mfilename('fullpath'));
    % Give source cpp filename
    if ~isequal(fileparts(which('control')), dir)
        % Compile the mex
        cwd = cd(dir);
        cleanup_obj = onCleanup(@() cd(cwd));
        fprintf('Compiling example_mex\n');
        mex example_mex.cpp
    end
    
    fprintf('Using SimulationInterface\n');

    % Give source cpp filename
    obj = mex_interface(str2fun([dir '/pansim'])); % str2fun allows us to use the full path, so the mex need not be on our path
    
    % Initially give an array of strings
    initOptions = ["opt1", "opt2", "opt3", "opt4"]
    runOptions = ["opt5", "opt6"]
    obj.initSimulation(initOptions);
    simTimeInDays = 10;
    t = 0;
    while t < simTimeInDays
        stats = obj.runForDayInMatlab(runOptions)
        % Do something with stats
        t = t + 1;
    end
    
    
    clear obj % Clear calls the delete method
end